#pragma once
#include <vector>
#include <string>
#include "RTDE.h" 

class DataFileHandler {
public:

    static bool saveToCSV(const std::vector<RobotData>& data, const std::string& filename);

    static bool loadFromCSV(const std::string& filename, std::vector<RobotData>& data);

private:
    // 私有辅助函数，用于格式化 double 值，确保精度
    static std::string formatDouble(double value);
};

