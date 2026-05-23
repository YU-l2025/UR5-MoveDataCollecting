#include "stdafx.h" 
#include "data_process.h"

// transmatrix 实现
transmatrix::transmatrix() {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            data[r][c] = (r == c) ? 1.0 : 0.0;
        }
    }
}

transmatrix transmatrix::operator*(const transmatrix& other) const {
    transmatrix result;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            result.data[r][c] = 0;
            for (int k = 0; k < 4; k++) {
                result.data[r][c] += data[r][k] * other.data[k][c];
            }
        }
    }
    return result;
}

// KinematicsEngine 实现
Data_Process::Data_Process() {
    // 初始化 UR5 D-H 参数 
    m_dhParams = {
        {0,         0.089159,   0,          M_PI / 2},
        {0,         0,          -0.425,     0},
        {0,         0,          -0.39225,   0},
        {0,         0.10915,    0,          M_PI / 2},
        {0,         0.09465,    0,          -M_PI / 2},
        {0,         0.0823,     0,          0}
    };
}

double Data_Process::pulseToJointAngle(int numByte32Buf, int bit, double zero_point, double deceleration_rate, double start_pulse) {
    long long bits = (1LL << bit);
    double joint_angle = ((start_pulse + numByte32Buf) * 360.0 / bits - zero_point) / deceleration_rate;
    return joint_angle; // 返回角度值
}

transmatrix Data_Process::solveForwardKinematics(const std::vector<double>& jointAngles) {
    transmatrix T_total; // 初始为单位阵

    for (int i = 0; i < 6; ++i) {
        double theta = jointAngles[i] + m_dhParams[i].theta;
        double d = m_dhParams[i].d;
        double a = m_dhParams[i].a;
        double alpha = m_dhParams[i].alpha;

        transmatrix Ti;
        // 标准 D-H 变换矩阵公式
        Ti.data[0][0] = cos(theta);  Ti.data[0][1] = -sin(theta) * cos(alpha); Ti.data[0][2] = sin(theta) * sin(alpha);  Ti.data[0][3] = a * cos(theta);
        Ti.data[1][0] = sin(theta);  Ti.data[1][1] = cos(theta) * cos(alpha);  Ti.data[1][2] = -cos(theta) * sin(alpha); Ti.data[1][3] = a * sin(theta);
        Ti.data[2][0] = 0;           Ti.data[2][1] = sin(alpha);               Ti.data[2][2] = cos(alpha);              Ti.data[2][3] = d;
        Ti.data[3][0] = 0;           Ti.data[3][1] = 0;                        Ti.data[3][2] = 0;                       Ti.data[3][3] = 1;

        T_total = T_total * Ti;
    }
    return T_total;
}

Pose Data_Process::getPosition(const transmatrix& T) {
    Pose res;
    // 1. 提取位置 (单位转为 mm)
    res.x = T.data[0][3] * 1000.0;
    res.y = T.data[1][3] * 1000.0;
    res.z = T.data[2][3] * 1000.0;

    // 2. 提取姿态 (以 Z-Y-X 欧拉角为例)
    // 注意：这里需要根据你选用的旋转顺序调整公式
    double sy = sqrt(T.data[0][0] * T.data[0][0] + T.data[1][0] * T.data[1][0]);
    bool singular = sy < 1e-6; // 判定奇异值（万向锁）

    if (!singular) {
        res.w = atan2(T.data[1][0], T.data[0][0]) * 180.0 / M_PI;
        res.p = atan2(-T.data[2][0], sy) * 180.0 / M_PI;
        res.r = atan2(T.data[2][1], T.data[2][2]) * 180.0 / M_PI;
    }
    else {
        res.w = 0;
        res.p = atan2(-T.data[2][0], sy) * 180.0 / M_PI;
        res.r = atan2(-T.data[1][2], T.data[1][1]) * 180.0 / M_PI;
    }
    return res;
}