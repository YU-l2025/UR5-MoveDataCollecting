#include "stdafx.h"
#include "customshow.h"
#include <QHBoxLayout>
#include <QMouseEvent>
#include <cmath>
#include <QDebug>

CustomShow::CustomShow(QWidget* parent) : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(10);

    // 创建图表
    m_plot = new QCustomPlot(this);
    setupPlot();
    mainLayout->addWidget(m_plot, 1);

    // 创建滑动条控制区
    setupSliderControl();
    mainLayout->addWidget(m_timeLabel);

    setLayout(mainLayout);
}

void CustomShow::setupPlot()
{
    //// 设置坐标轴
    //m_plot->xAxis->setLabel("Index");
    //m_plot->yAxis->setLabel("Angle");
    //m_plot->xAxis->setTickLabelFont(QFont("Arial", 9));
    //m_plot->yAxis->setTickLabelFont(QFont("Arial", 9));

    // 网格
    m_plot->xAxis->grid()->setVisible(true);
    m_plot->yAxis->grid()->setVisible(true);
    m_plot->xAxis->grid()->setPen(QPen(QColor(200, 200, 200), 0.5));
    m_plot->yAxis->grid()->setPen(QPen(QColor(200, 200, 200), 0.5));
    m_plot->yAxis->setRange(-180, 180);

    // 创建6条曲线
    QVector<QColor> colors = {
        QColor(230, 74, 25),   // J1
        QColor(41, 121, 255),  // J2
        QColor(46, 204, 113),  // J3
        QColor(155, 89, 182),  // J4
        QColor(241, 196, 15),  // J5
        QColor(26, 188, 156)   // J6
    };

    QStringList names = { "J1", "J2", "J3", "J4", "J5", "J6" };

    for (int i = 0; i < 6; ++i) {
        auto graph = m_plot->addGraph();
        graph->setName(names[i]);
        graph->setPen(QPen(colors[i], 1.5));
        m_jointGraphs.append(graph);
    }

    // 图例
    m_plot->legend->setVisible(true);
    m_plot->legend->setFont(QFont("Arial", 8));
    m_plot->legend->setBrush(QColor(255, 255, 255, 220));

    // 指示线
    m_indicatorLine = new QCPItemStraightLine(m_plot);
    m_indicatorLine->point1->setCoords(0, -200);
    m_indicatorLine->point2->setCoords(0, 200);
    m_indicatorLine->setPen(QPen(QColor(255, 215, 0), 2, Qt::DashLine)); // 金色虚线

    // 交互设置
    m_plot->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);   // 曲线可选中

    // “框选缩放”模式
    m_plot->setSelectionRectMode(QCP::srmZoom);

    if (auto* rect = m_plot->selectionRect())
    {
        rect->setPen(QPen(QColor(0, 120, 255), 1, Qt::DashLine));
        rect->setBrush(QBrush(QColor(0, 120, 255, 30)));   // 半透明填充
    }    

    for (int i = 0; i < 6; ++i)
    {
        if (m_jointGraphs[i])
        {
            // 1. 开启自适应采样（数据量大时自动降采样，性能提升巨大！）
            m_jointGraphs[i]->setAdaptiveSampling(true);   // QCustomPlot 2.x 以上必须
            // 2. 强制使用直线（避免默认的样条曲线计算）
            m_jointGraphs[i]->setLineStyle(QCPGraph::lsLine);
            // 3. 关闭散点（散点绘制非常耗 CPU）
            m_jointGraphs[i]->setScatterStyle(QCPScatterStyle::ssNone);
        }
    }
}

void CustomShow::setupSliderControl()
{    
    m_timeLabel = new QLabel("Time: 0.00 s", this);
    m_timeLabel->setAlignment(Qt::AlignCenter);
    m_timeLabel->setFont(QFont("Arial", 9, QFont::Bold));

}


void CustomShow::addDataPoints(const std::vector<RobotData>& datas)
{
    if (datas.empty()) return;

    for (const auto& data : datas)
    {
        double x = static_cast<double>(data.globalIndex);
        for (int i = 0; i < 6 && i < data.joints.size(); ++i)
        {
            double y = data.joints[i] * 180.0 / M_PI;
            m_jointGraphs[i]->addData(x, y);  // true = 自动排序
        }
        m_lastAddedIndex = data.globalIndex;
    }
}

void CustomShow::updateshow()
{
    if (!m_plot) return;
    size_t index = getCurrentIndex();

    //qDebug() << "index:" << index;

    double x = static_cast<double>(index);
    double time = x / 100;
    m_timeLabel->setText(QString("time: %1 s").arg(time, 0, 'f', 3));
    m_plot->rescaleAxes(false);        // false = 只缩放不重置范围
    m_plot->replot(QCustomPlot::rpQueuedReplot);
}

void CustomShow::setCurrentIndex(size_t index)
{
    if (!m_plot || m_jointGraphs[0]->dataCount() == 0) return;

    // 找到该 index 对应的 X 坐标（globalIndex）
    double x = static_cast<double>(index);
    double time = x / 100;
    m_timeLabel->setText(QString("time: %1 s").arg(time, 0, 'f', 3));

    // 自动滚动到游标附近可见
    m_indicatorLine->point1->setCoords(x, -180);
    m_indicatorLine->point2->setCoords(x, 180);
    //m_plot->xAxis->setRange(x - 50, x + 150);   // 可自行调整视野宽度
    m_plot->replot(QCustomPlot::rpQueuedReplot);
}
void CustomShow::cleargraphs(void) 
{
    m_plot->clearGraphs();
    m_jointGraphs.clear();
    QVector<QColor> colors = {
    QColor(230, 74, 25),   // J1
    QColor(41, 121, 255),  // J2
    QColor(46, 204, 113),  // J3
    QColor(155, 89, 182),  // J4
    QColor(241, 196, 15),  // J5
    QColor(26, 188, 156)   // J6
    };

    QStringList names = { "J1", "J2", "J3", "J4", "J5", "J6" };

    for (int i = 0; i < 6; ++i) {
        auto graph = m_plot->addGraph();
        graph->setName(names[i]);
        graph->setPen(QPen(colors[i], 1.5));
        m_jointGraphs.append(graph);
    }
}
CustomShow::~CustomShow() = default;