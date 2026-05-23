#include "stdafx.h" 
#include "datafile.h"

#include <fstream>
#include <sstream>
#include <iostream>

// 辅助函数：格式化 double，防止科学计数法并保证精度
std::string DataFileHandler::formatDouble(double value) {
    std::ostringstream oss;
    oss.precision(15); // 设置高精度
    oss << std::fixed << value;
    return oss.str();
}

bool DataFileHandler::saveToCSV(const std::vector<RobotData>& data, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return false;
    }

    // 写入CSV头部
    file << "global_index,timestamp_ms,tcp_x,tcp_y,tcp_z,tcp_rx,tcp_ry,tcp_rz,joint_q1,joint_q2,joint_q3,joint_q4,joint_q5,joint_q6\n";

    for (const auto& item : data) {
        file << item.globalIndex << ","
            << item.timestampMs << ",";

        // TCP Pose (6个值)
        for (int i = 0; i < 6; ++i) {
            file << formatDouble(item.tcpPose.at(i));
            if (i < 5) file << ",";
        }
        file << ",";

        // Joint Angles (6个值)
        for (int i = 0; i < 6; ++i) {
            file << formatDouble(item.joints.at(i));
            if (i < 5) file << ",";
        }
        file << "\n";
    }

    file.close();
    std::cout << "Data successfully saved to " << filename << std::endl;
    return true;
}

bool DataFileHandler::loadFromCSV(const std::string& filename, std::vector<RobotData>& data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for reading: " << filename << std::endl;
        return false;
    }

    data.clear(); // 清空现有数据
    std::string line;
    bool isHeader = true;

    while (std::getline(file, line)) {
        if (isHeader) {
            isHeader = false; // 跳过第一行（头部）
            continue;
        }

        std::istringstream iss(line);
        std::string token;
        RobotData item;

        // 解析 global_index
        if (!std::getline(iss, token, ',')) return false;
        try { item.globalIndex = static_cast<size_t>(std::stoull(token)); }
        catch (...) { return false; }

        // 解析 timestamp_ms
        if (!std::getline(iss, token, ',')) return false;
        try { item.timestampMs = static_cast<qint64>(std::stoll(token)); }
        catch (...) { return false; }

        // 解析 tcpPose (6个值)
        item.tcpPose.resize(6);
        for (int i = 0; i < 6; ++i) {
            if (!std::getline(iss, token, ',')) return false;
            try { item.tcpPose[i] = std::stod(token); }
            catch (...) { return false; }
        }

        // 解析 joints (6个值)
        item.joints.resize(6);
        for (int i = 0; i < 6; ++i) {
            if (i < 5) { // 读取前5个逗号分隔的值
                if (!std::getline(iss, token, ',')) return false;
            }
            else { // 最后一个值后面没有逗号
                if (!std::getline(iss, token)) break;
            }
            try { item.joints[i] = std::stod(token); }
            catch (...) { return false; }
        }

        data.push_back(item);
    }

    file.close();
    std::cout << "成功加载： " << filename << ". Loaded " << data.size() << " entries." << std::endl;
    return true;
}