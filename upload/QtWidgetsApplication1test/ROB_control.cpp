#include "stdafx.h"
#include "ROB_control.h"


ControlWorker::ControlWorker(ur_rtde::RTDEControlInterface* control_iface, QObject* parent)
    : QObject(parent), m_rtde_control(control_iface), m_running(true)
{
}

ControlWorker::~ControlWorker()
{
    stop();
}

void ControlWorker::stop()
{
    m_running = false;
    m_cond.notify_all(); // 唤醒可能在等待的线程，使其安全退出
}

void ControlWorker::addCommand(const ControlCommand& cmd)
{
    if (!m_running) return;

    {
        //qDebug() << "add command";
        std::lock_guard<std::mutex> lock(m_mutex);

        // 核心优化：清空旧指令，只保留最新的一条
        // 这样滑块快速拖动时，机器人不会因为排队执行历史指令而产生“迟滞感”
        std::queue<ControlCommand> empty;
        std::swap(m_commandQueue, empty);

        m_commandQueue.push(cmd);
    }
    m_cond.notify_one(); // 唤醒控制循环
}

void ControlWorker::setControlInterface(ur_rtde::RTDEControlInterface* control_iface)
{
    // 加锁，防止在 process 运行时强行替换指针
    std::lock_guard<std::mutex> lock(m_mutex);
    m_rtde_control = control_iface;
    qDebug() << "[ControlWorker] Control interface pointer updated.";
}

void ControlWorker::process()
{
    qDebug() << "[ControlWorker] Started on thread:" << QThread::currentThreadId();

    while (1) {
        ControlCommand currentCmd;

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            // 睡眠等待，直到队列有数据 或 要求停止
            m_cond.wait(lock, [this]() {
                return !m_commandQueue.empty() || !m_running;
                });

            if (!m_running) break;

            currentCmd = m_commandQueue.front();
            m_commandQueue.pop();
        } // 此时 lock 自动释放

        // 执行真正的机器人运动控制
        if (m_running && m_rtde_control && m_rtde_control->isConnected()) {
            try {
                m_rtde_control->moveJ(currentCmd.q, currentCmd.speed, currentCmd.acc, true);
            }
            catch (const std::exception& e) {
                qDebug() << "[ControlWorker] moveJ Exception:" << e.what();
            }
        }
    }

    qDebug() << "[ControlWorker] Stopped.";
}