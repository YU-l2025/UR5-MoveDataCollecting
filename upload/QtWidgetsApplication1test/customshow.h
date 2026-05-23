#pragma once
#ifndef CUSTOMSHOW_H
#define CUSTOMSHOW_H

#include "qcustomplot.h"
#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <vector>
#include <QMutex>
#include "RTDE.h"

class CustomShow : public QWidget
{
    Q_OBJECT

public:
    explicit CustomShow(QWidget* parent = nullptr);
    ~CustomShow() override;

    // 添加数据点
    void addDataPoints(const std::vector<RobotData>& datas);
    void setCurrentIndex(size_t index);
    size_t getCurrentIndex() const { return m_lastAddedIndex; }

    void cleargraphs(void);

signals:
    // 滑动条移动信号
    void sliderIndexChanged(size_t index);

public slots:
    void updateshow();

private:
    void setupPlot();
    void setupSliderControl();

    // UI组件
    QCustomPlot* m_plot;
    QLabel* m_timeLabel;

    // 数据缓存
    QMutex m_dataMutex;
    std::vector<RobotData> m_historyData; // 历史数据
    size_t m_currentIndex = 0;

    // QCustomPlot元素
    QCPItemStraightLine* m_indicatorLine; // 指示线
    QVector<QCPGraph*> m_jointGraphs;     // 6条关节曲线
    static constexpr int JOINT_COUNT = 6;

    long long m_lastAddedIndex = -1;

};

#endif // CUSTOMSHOW_H
