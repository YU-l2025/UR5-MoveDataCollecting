#pragma once
#include <QObject>     
#include <atomic>
#include <ur_rtde/rtde.h>
#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <ur_rtde/rtde_control_interface.h>


    // 控制指令结构体
struct ControlCommand {
    std::vector<double> q; // 弧度
    double speed;
    double acc;
};



// ControlWorker.h
class ControlWorker : public QObject {
    Q_OBJECT
public:
    explicit ControlWorker(ur_rtde::RTDEControlInterface* control_iface, QObject* parent = nullptr);
    
    ~ControlWorker();

    // 添加控制指令（供外部线程调用，线程安全）
    void addCommand(const ControlCommand& cmd);

    // 停止运行标志（析构或断开连接时调用）
    void stop();

    void setControlInterface(ur_rtde::RTDEControlInterface* control_iface);

public slots:
    void process();

private:
    ur_rtde::RTDEControlInterface* m_rtde_control;
    std::queue<ControlCommand> m_commandQueue;
    std::atomic<bool> m_running;
    std::mutex m_mutex;
    std::condition_variable m_cond;
};