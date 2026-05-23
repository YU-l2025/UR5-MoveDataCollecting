#pragma once

#include <string>
#include <vector>
#include <memory>
#include <deque>
#include <queue>
#include <mutex>
#include <condition_variable>

#include <ur_rtde/rtde.h>
#include <ur_rtde/rtde_control_interface.h>
#include <ur_rtde/rtde_receive_interface.h>

#include <QObject>     
#include <QThread>      // 线程类
#include <QMutex>       // 互斥锁，保护数据
#include <atomic>       // 用于原子操作标志位
#include <QMetaType> 
#include "ROB_control.h"

struct RobotData {
    std::vector<double> joints;           // 6轴关节角 (弧度)
    std::vector<double> tcpPose;          // TCP位姿 [x,y,z,rx,ry,rz] (米/弧度)
    qint64 timestampMs;                   // 时间戳 (毫秒)
    size_t globalIndex;                   // 全局索引 (用于滑动条同步)

    RobotData() : timestampMs(0), globalIndex(0) {}
    RobotData(const std::vector<double>& j, const std::vector<double>& p, qint64 ts, size_t idx)
        : joints(j), tcpPose(p), timestampMs(ts), globalIndex(idx) {
    }
};

Q_DECLARE_METATYPE(RobotData)

class RTDE : public QObject
{
    Q_OBJECT
public:
    RTDE();
    ~RTDE();

    bool connect(const std::string& robot_ip);
    void disconnect();

    // 当前关节角度
    std::vector<double> getActualQ();

    // 获取 TCP 位姿
    std::vector<double> getActualTCPPose();

    bool isConnected() const;

    // 开始/停止高频采集
    void startCollecting();
    void stopCollecting();

    //获取当前最新索引
    size_t getCurrentIndex() const { return m_globalIndex.load(); }

    int getRobotDataSegment(size_t startIdx, size_t endIdx, std::vector<RobotData>& outData);
    size_t getCurrentMaxIndex() const;

    RobotData getDataByIndex(size_t index) const;

    //  连接标志位
    bool connected = false;
    //  采集按键标志位
    bool collect_state = false;

    // 新增：异步控制接口 (度为单位，方便与UI对接)
    void moveJ_Async(const std::vector<double>& jointsDeg, double speed = 1.0, double acc = 1.0);

    void stopControlThread();
signals:
    void newDataAvailable(const RobotData& data);
    void dataRangeChanged(size_t minIndex, size_t maxIndex);

private:
    //  接收/控制变量
    ur_rtde::RTDEControlInterface* rtde_control;
    ur_rtde::RTDEReceiveInterface* rtde_receive;

    // 多线程相关
    QThread* m_workerThread;     // 工作线程指针
    std::atomic<bool> m_running; // 原子布尔值，控制循环停止，线程安全
    mutable QMutex m_dataMutex;  // 保护内部数据不被读写冲突
    //  缓存数据
    std::deque<std::vector<double>> m_historyJoints;
    std::deque<std::vector<double>> m_historyPose;
    //最大数据量
    const size_t MAX_HISTORY_SIZE = 2000;
    //  全局计数器
    std::atomic<size_t> m_globalIndex{ 0 };

    //数据缓存
    mutable QMutex m_dataCacheMutex;
    std::map<size_t, RobotData> m_dataCache; // 全局索引 -> 数据

    //数据统计
    size_t m_minIndex = 0;
    size_t m_maxIndex = 0;

    //缓存大小限制
    size_t m_maxCacheSize = 10000; // 限制最大缓存10000条数据

    // 控制线程相关

    ControlWorker* m_ctrlWorker;
    //QThread* m_ctrlThread;

    QThread* m_controlThread;
    //std::atomic<bool> m_controlRunning;
    //// 控制指令结构体
    //struct ControlCommand {
    //    std::vector<double> q; // 弧度
    //    double speed;
    //    double acc;
    //};
    //std::queue<ControlCommand> m_commandQueue;
    //std::mutex m_cmdMutex;
    //std::condition_variable m_cmdCond; // 用于唤醒控制循环

    //  采集函数
    void collectionLoop();
    // 控制线程执行函数
    //void controlLoop();

};
