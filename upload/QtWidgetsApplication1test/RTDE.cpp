#include "stdafx.h"
#include "RTDE.h"


#include <QDebug>
#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>

using namespace std;

RTDE::RTDE() : QObject(), connected(false)
{
    m_workerThread = new QThread(this);
    m_running = false;

    // 将当前对象移动到子线程
    this->moveToThread(m_workerThread);

    // 连接线程启动信号到工作函数
    QObject::connect(m_workerThread, &QThread::started, this, &RTDE::collectionLoop);
    //  启动进入循环 等待采集标记
    m_workerThread->start();

    // 初始化控制线程


    //m_controlThread = new ControlWorker(rtde_control);
    //m_controlThread = new QThread();
    //m_controlThread->moveToThread(m_controlThread);
    //connect(m_controlThread, &QThread::started, m_ctrlWorker, &ControlWorker::process);
    //m_ctrlThread->start();
    m_ctrlWorker = new ControlWorker(nullptr); // 不要传 this 作为父对象
    m_controlThread = new QThread();
    m_ctrlWorker->moveToThread(m_controlThread);
    // 线程启动时，触发工人的 process 函数
    QObject::connect(m_controlThread, &QThread::started, m_ctrlWorker, &ControlWorker::process);

    // 线程结束时，自动清理 worker 的内存（防止内存泄漏）
    QObject::connect(m_controlThread, &QThread::finished, m_ctrlWorker, &QObject::deleteLater);

    m_controlThread->start();

}
RTDE::~RTDE() {
    stopCollecting();
    //  预防性处理
    timeEndPeriod(1);

    //  线程清理
    if (m_workerThread) {
        m_workerThread->quit();
        m_workerThread->wait();
        delete m_workerThread;
        m_workerThread = nullptr;
    }
    // 停止并清理控制线程
    stopControlThread();

    if (m_controlThread->isRunning()) {
        m_controlThread->quit();
        m_controlThread->wait();
        delete m_controlThread;
        m_controlThread = nullptr;
    }

    disconnect();
}

bool RTDE::connect(const std::string& robot_ip) {
    try {
        // 接收接口
        rtde_receive = new ur_rtde::RTDEReceiveInterface(robot_ip);

         //控制接口
         rtde_control = new ur_rtde::RTDEControlInterface(robot_ip);

        if (rtde_receive->isConnected() && rtde_control->isConnected()) {
            connected = true;
            if (m_ctrlWorker) {
                m_ctrlWorker->setControlInterface(rtde_control);
            }
            qDebug() << "RTDE 连接成功:" << QString::fromStdString(robot_ip);
            return true;
        }
    }
    catch (const std::exception& e) {
        qDebug() << "RTDE 连接失败:" << e.what();
    }
    return false;
}

void RTDE::disconnect() {
    if (connected == true) {
        if (rtde_receive) {
            rtde_receive->disconnect();
            stopCollecting();
        }
        if (rtde_control) {
            rtde_control->disconnect();
        }
        connected = false;
    }
    else {
        return;
    }
}

bool RTDE::isConnected() const { return connected; }

std::vector<double> RTDE::getActualQ() {
    if (!rtde_receive || !connected) return {};
    QMutexLocker locker(&m_dataMutex);
    return rtde_receive->getActualQ();
}

std::vector<double> RTDE::getActualTCPPose() {
    if (!rtde_receive || !connected) return {};
    QMutexLocker locker(&m_dataMutex);
    return rtde_receive->getActualTCPPose();
}


void RTDE::startCollecting() {
    if (m_running) return;
    m_running.store(true);
    qDebug() << "[RTDE Thread] Data acquisition STARTED.";
}

void RTDE::stopCollecting() {
    if (!m_running) return;
    m_running = false;
    qDebug() << "[RTDE Thread] Data acquisition STOPPED.";
}

void RTDE::collectionLoop() {

    qDebug() << "[RTDE Thread] Worker loop initialized on thread:" << QThread::currentThreadId();
    bool isHighResTimerEnabled = false; //  标记
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();
    int size = 0;
    int count = 0;
    // 模拟螺旋线参数
    double m_simTime=0.0;
    double radius = 0.2;       // 半径 0.2米
    double heightSpeed = 0.005; // 每帧上升高度 (米)
    double angularSpeed = 0.1;  // 每帧旋转弧度

    while (true) {
        if (m_running.load()){
            if (!isHighResTimerEnabled) {
                if (timeBeginPeriod(1) == TIMERR_NOERROR) {
                    isHighResTimerEnabled = true;
                    qDebug() << "[RTDE] High resolution timer (1ms) ENABLED.";
                }
                else {
                    qDebug() << "[RTDE] Failed to enable high resolution timer.";
                }
            }
            if (!connected) {
                // 如果断开了，休眠一下避免空转
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                continue;
            }

            // 执行数据读取
            std::vector<double> currentPose = getActualQ();
            std::vector<double> tcpPose = getActualTCPPose();
            //  模拟
            //double x = radius * cos(m_simTime);
            //double y = radius * sin(m_simTime);
            //double z = 0.03 + (m_simTime * heightSpeed); // 从0.3米开始向上螺旋
            //std::vector<double> tcpPose = { x,y,z,0,0,0 };
            //std::vector<double> currentPose = {
            // 0.1 * sin(m_simTime), 0.2 * cos(m_simTime), 0.1 * sin(m_simTime * 2),
            // 0.1 * cos(m_simTime), 0.1 * sin(m_simTime), 0.1 * cos(m_simTime * 2)
            //};
            // 【关键】创建数据结构体
            RobotData data;
            data.joints = currentPose;
            data.tcpPose = tcpPose;
            data.timestampMs = QDateTime::currentMSecsSinceEpoch();
            data.globalIndex = m_globalIndex.load();

            // 【关键】存入缓存
            {
                QMutexLocker locker(&m_dataCacheMutex);
                m_dataCache[m_globalIndex] = data;
                // 更新范围
                if (m_globalIndex == 0) {
                    m_minIndex = m_globalIndex;
                }
                m_maxIndex = m_globalIndex;

                //// 存储数据
                //m_dataCache[m_globalIndex] = data;

                //// 限制缓存大小（滑动窗口）
                //if (m_dataCache.size() > m_maxCacheSize) {
                //    auto it = m_dataCache.begin();
                //    m_dataCache.erase(it);
                //    m_minIndex = m_dataCache.begin()->first;
                //}
            }


            // 更新全局索引
            m_globalIndex++;

            m_simTime += angularSpeed;

            // 调试输出
            //if (count == 50) { // 每50次输出一次
            //    QMutexLocker locker(&m_dataCacheMutex);
            //    qDebug() << "Cached data range:" << m_minIndex << "to" << m_maxIndex
            //        << "Total:" << m_dataCache.size()
            //        <<"tcp:"<<data.tcpPose
            //        <<"angle:"<<data.joints;
            //    count = 0;
            //}
            //count++;

            //{
            //    //  全局计数+1
            //    m_globalIndex++;
            //    // 【新增】滑动窗口 (防止长时间运行内存爆炸)
            //    //if (m_historyPose.size() > MAX_HISTORY_SIZE) {
            //    //    m_historyJoints.pop_front();
            //    //    m_historyPose.pop_front();
            //    //}
            //}

            // 控制频率：UR 机器人通常 500Hz，这里设为 100Hz (10ms) 
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        else {
            if (isHighResTimerEnabled) {
                timeEndPeriod(1);
                isHighResTimerEnabled = false;
                qDebug() << "[RTDE] High resolution timer RELEASED.";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    //  预防性
    if (isHighResTimerEnabled) {
        timeEndPeriod(1);
    }
}



// RTDE数据获取函数
int RTDE::getRobotDataSegment(size_t startIdx, size_t endIdx, std::vector<RobotData>& outData)
{
    QMutexLocker locker(&m_dataCacheMutex);

    if (startIdx > endIdx || startIdx < m_minIndex || endIdx > m_maxIndex) {
        return -1; // 参数错误
    }

    outData.clear();
    for (size_t i = startIdx; i <= endIdx && i <= m_maxIndex; ++i) {
        auto it = m_dataCache.find(i);
        if (it != m_dataCache.end()) {
            outData.push_back(it->second);
        }
    }

    return static_cast<int>(outData.size()); // 返回实际获取的数据点数量
}

size_t RTDE::getCurrentMaxIndex() const
{
    QMutexLocker locker(&m_dataCacheMutex);
    return m_maxIndex;
}
//获取指定索引的数据
RobotData RTDE::getDataByIndex(size_t index) const {
    QMutexLocker locker(&m_dataCacheMutex);
    auto it = m_dataCache.find(index);
    if (it != m_dataCache.end()) {
        return it->second;
    }
    return RobotData(); // 返回空数据
}
//
//// 【新增】获取数据范围
//std::pair<size_t, size_t> RTDE::getDataRange() const {
//    QMutexLocker locker(&m_dataCacheMutex);
//    return std::make_pair(m_minIndex, m_maxIndex);
//}
//
//// 【新增】设置缓存大小
//void RTDE::setMaxCacheSize(size_t maxSize) {
//    m_maxCacheSize = maxSize;
//}

// UI 调用的接口：将指令压入队列
void RTDE::moveJ_Async(const std::vector<double>& jointsDeg, double speed, double acc) {
    if (!connected || !rtde_control) return;

    ControlCommand cmd;
    for (double deg : jointsDeg) {
        cmd.q.push_back(deg * M_PI / 180.0); // 转换为弧度
    }
    cmd.speed = speed;
    cmd.acc = acc;

    //{
    //    std::lock_guard<std::mutex> lock(m_cmdMutex); // 使用 std::lock_guard
    //    m_commandQueue.push(cmd);
    //}
    m_ctrlWorker->addCommand(cmd);
    //m_cmdCond.notify_one();
}

// 控制线程的内部循环
//void RTDE::controlLoop() {
//    qDebug() << "[RTDE Control] Thread started:" << QThread::currentThreadId();
//
//    while (1) {
//        ControlCommand currentCmd;
//
//        {
//            std::unique_lock<std::mutex> lock(m_cmdMutex);
//            //QMutexLocker locker(&m_commandMutex);
//            // 等待直到有指令或线程停止
//            m_cmdCond.wait(lock, [this]() {
//                return !m_commandQueue.empty() || !m_controlRunning;
//                });
//
//            if (!m_controlRunning) break;
//
//            currentCmd = m_commandQueue.front();
//            m_commandQueue.pop();
//        }
//
//        // 执行真正的 moveJ 指令
//        //try {
//        //    if (rtde_control && rtde_control->isConnected()) {
//        //        // moveJ 的最后一个参数设为 true 表示异步执行（在机器人端非阻塞）
//        //        // 但因为我们在独立线程调用，可以根据需要设为 false 让线程等待动作完成
//        //        rtde_control->moveJ(currentCmd.q, currentCmd.speed, currentCmd.acc);
//        //    }
//        //}
//        //catch (const std::exception& e) {
//        //    qDebug() << "[RTDE Control] MoveJ Error:" << e.what();
//        //}
//        if (m_controlRunning && rtde_control && rtde_control->isConnected()) {
//            try {
//                // 执行 moveJ
//                rtde_control->moveJ(currentCmd.q, currentCmd.speed, currentCmd.acc);
//            }
//            catch (const std::exception& e) {
//                qDebug() << "RTDE Control Error:" << e.what();
//            }
//        }
//    }
//    qDebug() << "[RTDE Control] Thread stopped.";
//}

void RTDE::stopControlThread() 
{
    //{
    //    std::lock_guard<std::mutex> lock(m_cmdMutex);
    //    m_controlRunning = false; // 修改原子标志
    //}
    //m_cmdCond.notify_all(); // 唤醒可能正在 wait 的控制线程
    if (m_ctrlWorker) {
        // 先通知 worker 退出死循环
        m_ctrlWorker->stop();
    }
}

