#ifndef DATA_PROCESS_H
#define DATA_PROCESS_H

#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 齐次变换矩阵类
class transmatrix {
public:
    double data[4][4];

    transmatrix();
    // 重载乘法运算符，实现矩阵连乘
    transmatrix operator*(const transmatrix& other) const;
};

// D-H 参数结构体
struct DHParam {
    double theta; // 偏置角度 (弧度)
    double d;     // 连杆偏距 (m)
    double a;     // 连杆长度 (m)
    double alpha; // 连杆扭角 (弧度)
};

struct Pose {
    double x, y, z;    // 位置 (mm)
    double w, p, r;    // 姿态 (角度)
};

class Data_Process {
public:
    // 构造函数，初始化 D-H 参数
    Data_Process();

    // 1. 伺服脉冲转关节角度 
    double pulseToJointAngle(int numByte32Buf, int bit, double zero_point, double deceleration_rate, double start_pulse);

    // 2. 正向运动学解算：输入 6 个关节角度 (弧度)，输出末端位姿矩阵
    transmatrix solveForwardKinematics(const std::vector<double>& jointAngles);

    // 获取坐标向量 [x, y, z]
    Pose getPosition(const transmatrix& T);

private:
    std::vector<DHParam> m_dhParams;
};

#endif // DATA_PROCESS_H
