#include "stdafx.h"
#include "robtrace_show.h"

using namespace std;

vtkStandardNewMacro(CustomStyle);

Qtrobtrace_show::Qtrobtrace_show(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    //  TRDE数据读取
    qRegisterMetaType<RobotData>("RobotData");
    rtde = new RTDE();
    plot = ui.customwidget;

    //  初始化
    initVtkWindowAndRenderer();
    setupCubeAxesActor();
    createHighlightAndText();
    setupOrientationMarker();
    setupCameraView();
    setupCustomInteractorStyle();
    QloadRobotModel();
    initTimer();
    initControlDock();

    renderWindow->Render();
}


Qtrobtrace_show::~Qtrobtrace_show()
{

}

//  TCP连接
void Qtrobtrace_show::connect_clicked()
{
    ui.IP->setText("192.168.0.1");

    if (!rtde->isConnected()) {
        bool cet = rtde->connect("192.168.0.1");

        if (cet == true) {
            ui.state_label->setText("连接成功");
        }
        else {
            ui.state_label->setText("连接失败");
        }
        return;
    }
    else
    {
        QMessageBox::critical(this, "错误", "机器人连接失败！");
        return;
    }
}
void Qtrobtrace_show::disconnect_clicked()
{
    rtde->disconnect();
    ui.control_dockWidget->hide();
}

//
void Qtrobtrace_show::collectdata()
{
    cout << "in collectdata";
}

//  导入数据文件
void Qtrobtrace_show::importfile_clicked() 
{
    qDebug() << "import file slot";
    // 让用户选择要加载的文件
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Load Robot Data"),
        QDir::homePath(),
        tr("CSV Files (*.csv);;All Files (*)")
    );

    if (fileName.isEmpty()) {
        return; // 用户取消了对话框
    }

    std::vector<RobotData> loadedData;
    if (!DataFileHandler::loadFromCSV(fileName.toLocal8Bit().constData(), loadedData)) {
        QMessageBox::critical(this, "Error", "导入文件失败\n" + fileName);
        return;
    }

    // 检查是否加载到了数据
    if (loadedData.empty()) {
        QMessageBox::information(this, "Info", "文件为空或数据不可用");
        return;
    }
    plot->cleargraphs();
    // 数据存储到 Qtrobtrace_show 自己的缓存中 ---
    m_loadedDataCache = loadedData; // 直接赋值
    m_isUsingLoadedData = true;     // 设置标志位

    // --- 更新界面显示 ---
    
    // 1. 重置VTK轨迹
    m_trajPoints->Reset();
    m_trajLines->Reset();
    m_lastDrawnIndex = 0; // 重置绘制索引
    m_boundsInitialized = false; // 重置边界初始化状态
    // 注意：m_currentBounds 应该在 setupCubeAxesActor 或类似函数中初始化一个默认值

    // 2. 将加载的数据批量添加到VTK和2D Plot
    // 注意：这里不再依赖实时的RTDE数据，而是使用 loadedData
    // 为了可视化，我们一次性处理所有加载的数据
    for (size_t i = 0; i < loadedData.size(); ++i) {
        const RobotData& data = loadedData[i];
        // 更新VTK轨迹点
        if (data.tcpPose.size() >= 3) {
            double x = data.tcpPose[0] * 1000.0;
            double y = data.tcpPose[1] * 1000.0;
            double z = data.tcpPose[2] * 1000.0;
            m_trajPoints->InsertNextPoint(x, y, z);

            // 更新范围逻辑 (与 updateTrajectory 中类似)
            if (!m_boundsInitialized) {
                m_currentBounds[0] = x; m_currentBounds[1] = x;
                m_currentBounds[2] = y; m_currentBounds[3] = y;
                m_currentBounds[4] = z; m_currentBounds[5] = z;
                m_boundsInitialized = true;
            }
            else {
                double margin = 50.0;
                if (x < m_currentBounds[0]) m_currentBounds[0] = x - margin;
                if (x > m_currentBounds[1]) m_currentBounds[1] = x + margin;
                if (y < m_currentBounds[2]) m_currentBounds[2] = y - margin;
                if (y > m_currentBounds[3]) m_currentBounds[3] = y + margin;
                if (z < m_currentBounds[4]) m_currentBounds[4] = z - margin;
                if (z > m_currentBounds[5]) m_currentBounds[5] = z + margin;
            }
        }

        // 更新VTK机器人姿态 (使用最后一个点的姿态或根据索引更新)
        if (data.joints.size() >= 6) {
            // 这里可以只更新最后一个点的姿态，或者根据需要实现动画
            // 为了演示，我们更新模型到第一个点的姿态
            if (i == 0) {
                moveRobotJoints(
                    data.joints[0] * 180.0 / M_PI,
                    data.joints[1] * 180.0 / M_PI,
                    data.joints[2] * 180.0 / M_PI,
                    data.joints[3] * 180.0 / M_PI,
                    data.joints[4] * 180.0 / M_PI,
                    data.joints[5] * 180.0 / M_PI
                );
            }
        }
    }

    // 更新VTK显示
    rebuildTrajectoryLines();
    m_trajData->Modified();
    if (m_boundsInitialized) {
        updateCubeAxesBounds();
    }
    renderWindow->Render();

    // 3. 更新2D Plot

    plot->addDataPoints(loadedData); // 添加新数据
    plot->updateshow(); // 触发重绘

    // 4. 更新时间滑动条
    ui.TimeSlider->setRange(0, static_cast<int>(loadedData.size() - 1));
    ui.TimeSlider->setValue(0); // 默认回到起点
    ui.TimeSlider->setEnabled(true);
    m_isPlaying = true; // 表示现在处于播放/查看模式

    QMessageBox::information(this, "Success", "导入文件成功\n" + fileName +
        "\n\nTotal entries: " + QString::number(loadedData.size()));

}
void Qtrobtrace_show::savefile_clicked()
{
    qDebug() << "save file slot";

    if (!rtde) {
        QMessageBox::warning(this, "Warning", "RTDE module not initialized.");
        return;
    }
    if (rtde->collect_state == false) {
        // 获取当前所有已缓存的数据
        size_t maxIdx = rtde->getCurrentMaxIndex();
        if (maxIdx == 0) {
            QMessageBox::information(this, "Info", "没有需保存的数据");
            return;
        }

        // 从 RTDE 缓存中提取所有数据
        std::vector<RobotData> dataToSave;
        int count = rtde->getRobotDataSegment(0, maxIdx, dataToSave);
        if (count <= 0) {
            QMessageBox::critical(this, "失败", "没有需保存的数据");
            return;
        }

        // 用户选择保存位置和文件名
        QString fileName = QFileDialog::getSaveFileName(
            this,
            tr("Save Robot Data"),
            QDir::homePath() + "/robot_data.csv", // 默认路径和文件名
            tr("CSV Files (*.csv);;All Files (*)")
        );

        if (fileName.isEmpty()) {
            return;
        }

        // 保存数据
        if (DataFileHandler::saveToCSV(dataToSave, fileName.toStdString())) {
            QMessageBox::information(this, "成功", "成功保存:\n" + fileName);
        }
        else {
            QMessageBox::critical(this, "失败", "保存失败:\n" + fileName);
        }
    }
    else
    {
        QMessageBox::warning(this, "Warning", "保存请先停止采集！");
        return;
    }

}

//  整体视角
void Qtrobtrace_show::fulltrace_clicked()
{
    setupCameraView();
    renderWindow->Render();   
    plot->updateshow();
}
//  采集按键槽函数
void Qtrobtrace_show::collect_clicked()
{
    if (rtde->isConnected()) {
        if (rtde->collect_state == false) {
            //emit start_collect();
            rtde->collect_state = true;
            rtde->startCollecting();
            ui.collect_button->setText("停止采集");
            m_isPlaying = false; // 退出回放模式
            if (!m_plotUpdateTimer->isActive())
                m_plotUpdateTimer->start(100);
        }
        else {
            //emit stop_collect();
            rtde->collect_state = false;
            rtde->stopCollecting();
            ui.collect_button->setText("开始采集");
            m_plotUpdateTimer->stop();
            plot->updateshow();
            // 启用 slider 并设置范围
            size_t maxIdx = rtde->getCurrentMaxIndex();

            if (maxIdx > 0) {
                ui.TimeSlider->setRange(0, static_cast<int>(maxIdx));
                ui.TimeSlider->setValue(static_cast<int>(maxIdx)); // 默认到最新位置
                ui.TimeSlider->setEnabled(true);
            }
            m_isPlaying = true;
        }
    }
    else{
        QMessageBox::critical(this, "错误", "连接机器人失败！");
        return;
    }
}

//  控制器显示
void Qtrobtrace_show::control_clicked()
{
    qDebug() << "control widget";
    if (rtde->isConnected()) {
        if (ui.control_dockWidget->isVisible()) {
            ui.control_dockWidget->hide();
        }
        else
        {
            ui.control_dockWidget->setVisible(true);
        }
    }
    else
    {
        QMessageBox::critical(this, "错误", "机器人未连接！");
    }
}

//  初始化VTK窗口
void Qtrobtrace_show::initVtkWindowAndRenderer()
{
    vtkWin = ui.VTKwidget;

    renderWindow = vtkGenericOpenGLRenderWindow::New();           // 成员是 raw pointer
    vtkWin->setRenderWindow(renderWindow);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    renderer->SetBackground(colors->GetColor3d("white").GetData());

    // 初始化轨迹相关VTK对象
    m_trajPoints = vtkSmartPointer<vtkPoints>::New();
    m_trajLines = vtkSmartPointer<vtkCellArray>::New();
    m_trajData = vtkSmartPointer<vtkPolyData>::New();
    m_trajData->SetPoints(m_trajPoints);
    m_trajData->SetLines(m_trajLines);

    m_trajMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_trajMapper->SetInputData(m_trajData);

    m_trajActor = vtkSmartPointer<vtkActor>::New();
    m_trajActor->SetMapper(m_trajMapper);
    m_trajActor->GetProperty()->SetColor(0.0, 0.8, 0.0); // 绿色
    m_trajActor->GetProperty()->SetLineWidth(3.0);
    renderer->AddActor(m_trajActor);

}

void Qtrobtrace_show::initTimer() 
{    // 初始化渲染定时器 (60Hz)
    m_renderTimer = new QTimer(this);
    connect(m_renderTimer, &QTimer::timeout, this, &Qtrobtrace_show::updateTrajectory);
    m_renderTimer->start(40); // ~60Hz  16

    m_lastDrawnIndex = 0;
    m_sampleStep = 30; // 500Hz -> 50Hz 显示
    m_cameraAutoAdjusted = false;

    m_plotUpdateTimer = new QTimer(this);
    connect(m_plotUpdateTimer, &QTimer::timeout, plot, &CustomShow::updateshow);

    connect(ui.TimeSlider, &QSlider::valueChanged, this, &Qtrobtrace_show::onPlaybackSliderChanged);
    ui.TimeSlider->setEnabled(false);
}
//  试创建曲线
void Qtrobtrace_show::createTrajectoryCurve()
{
    auto pts = vtkSmartPointer<vtkPoints>::New();
    for (double t = 0; t < 6.28; t += 0.1)
        pts->InsertNextPoint(cos(t), sin(t), t * 0.2);

    auto polyline = vtkSmartPointer<vtkPolyLine>::New();
    polyline->GetPointIds()->SetNumberOfIds(pts->GetNumberOfPoints());
    for (vtkIdType i = 0; i < pts->GetNumberOfPoints(); ++i)
        polyline->GetPointIds()->SetId(i, i);

    auto cells = vtkSmartPointer<vtkCellArray>::New();
    cells->InsertNextCell(polyline);

    polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(pts);
    polyData->SetLines(cells);

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polyData);

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0.0, 0.0, 0.8);
    actor->GetProperty()->SetLineWidth(4.0);

    renderer->AddActor(actor);
}

//  创建点击高亮球
void Qtrobtrace_show::createHighlightAndText()
{
    // 高亮球
    highlightSphere = vtkSmartPointer<vtkSphereSource>::New();
    highlightSphere->SetCenter(0, 0, 0);
    highlightSphere->SetRadius(1);
    highlightSphere->SetPhiResolution(20);
    highlightSphere->SetThetaResolution(20);
    highlightMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    highlightMapper->SetInputConnection(highlightSphere->GetOutputPort());
    highlightActor = vtkSmartPointer<vtkActor>::New();
    highlightActor->SetMapper(highlightMapper);
    highlightActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    highlightActor->SetVisibility(0);
    renderer->AddActor(highlightActor);
    // 设置期望屏幕大小（直径像素数）
    m_desiredScreenDiameter = 15.0;

    // 3D 文字
    coordTextActor = vtkSmartPointer<vtkBillboardTextActor3D>::New();
    coordTextActor->SetVisibility(0);
    coordTextActor->GetTextProperty()->SetFontSize(24);
    coordTextActor->GetTextProperty()->SetColor(1.0, 0.0, 0.0);
    coordTextActor->GetTextProperty()->BoldOn();
    renderer->AddActor(coordTextActor);

    // CellPicker
    cellPicker = vtkSmartPointer<vtkCellPicker>::New();
    cellPicker->SetTolerance(0.01);
}
void Qtrobtrace_show::updateHighlightSize()
{
    // 安全检查：对象有效 + 高亮球可见
    if (!highlightActor || !highlightActor->GetVisibility() ||
        !renderer || !renderWindow) return;

    vtkCamera* cam = renderer->GetActiveCamera();
    if (!cam) return;

    // 1. 获取高亮球世界坐标位置（单位：米）
    double worldPos[3];
    highlightActor->GetPosition(worldPos);

    // 2. 计算相机到球心的距离（米）
    double camPos[3];
    cam->GetPosition(camPos);
    double dx = camPos[0] - worldPos[0];
    double dy = camPos[1] - worldPos[1];
    double dz = camPos[2] - worldPos[2];
    double dist = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (dist < 1e-5) return; // 避免除零

    // 3. 获取窗口高度（像素）
    int* winSize = renderWindow->GetSize();
    double winHeight = (winSize[1] > 0) ? static_cast<double>(winSize[1]) : 600.0;

    // 4. 计算所需世界坐标半径（米）
    // 推导：屏幕直径(像素) = (2 * 世界半径 * 窗口高度) / (2 * 距离 * tan(视角/2))
    // => 世界半径 = (期望屏幕直径 * 距离 * tan(视角/2)) / 窗口高度
    double viewAngleRad = vtkMath::RadiansFromDegrees(cam->GetViewAngle());
    double worldRadius = (m_desiredScreenDiameter * dist * std::tan(viewAngleRad * 0.5)) / winHeight;

    // 5. 应用缩放（基础半径=1.0，缩放因子=目标半径）
    highlightActor->SetScale(worldRadius, worldRadius, worldRadius);
}

//  设置风格
void Qtrobtrace_show::setupCustomInteractorStyle()
{
    auto style = vtkSmartPointer<CustomStyle>::New();
    style->SetRenderer(renderer);
    style->SetPolyData(m_trajData);
    style->SetHighlightSphere(highlightSphere);
    style->SetHighlightActor(highlightActor);
    style->SetTextActor(coordTextActor);
    style->SetCellPicker(cellPicker);
    renderWindow->GetInteractor()->SetInteractorStyle(style);
}

//  创建左下角 图标指示器
void Qtrobtrace_show::setupOrientationMarker()
{
    axesActor = vtkSmartPointer<vtkAxesActor>::New();
    orientationWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    orientationWidget->SetOrientationMarker(axesActor);
    orientationWidget->SetInteractor(renderWindow->GetInteractor());
    orientationWidget->SetViewport(0.0, 0.0, 0.25, 0.25);
    orientationWidget->SetEnabled(1);
    orientationWidget->InteractiveOff();
}

//  设置网格坐标系
void Qtrobtrace_show::setupCubeAxesActor()
{
    double bounds[6] = { -1000.0, 1000.0, -1000.0, 1000.0, -1000.0, 1000.0 };
    m_boundsInitialized = false;
    cubeAxesActor = vtkSmartPointer<vtkCubeAxesActor>::New();
    cubeAxesActor->SetBounds(bounds);
    cubeAxesActor->SetCamera(renderer->GetActiveCamera());
    
    cubeAxesActor->DrawXGridlinesOn();
    cubeAxesActor->DrawYGridlinesOn();
    cubeAxesActor->DrawZGridlinesOn();
    cubeAxesActor->SetGridLineLocation(vtkCubeAxesActor::VTK_GRID_LINES_FURTHEST);
    cubeAxesActor->SetFlyModeToOuterEdges();

    // 颜色美化
    cubeAxesActor->GetXAxesGridlinesProperty()->SetColor(0.2, 0.2, 0.2);
    cubeAxesActor->GetYAxesGridlinesProperty()->SetColor(0.2, 0.2, 0.2);
    cubeAxesActor->GetZAxesGridlinesProperty()->SetColor(0.2, 0.2, 0.2);

    cubeAxesActor->SetGridLineLocation(vtkCubeAxesActor::VTK_GRID_LINES_FURTHEST);  // 网格线画在最外侧
    cubeAxesActor->SetFlyModeToOuterEdges(); // 坐标框固定在外边缘
    cubeAxesActor->GetTitleTextProperty(0)->SetColor(1.0, 0.0, 0.0);  // x轴标题红色
    cubeAxesActor->GetLabelTextProperty(0)->SetColor(1.0, 0.0, 0.0);
    cubeAxesActor->GetTitleTextProperty(1)->SetColor(0.0, 1.0, 0.0);  // y轴绿色
    cubeAxesActor->GetLabelTextProperty(1)->SetColor(0.0, 1.0, 0.0);
    cubeAxesActor->GetTitleTextProperty(2)->SetColor(0.0, 0.0, 1.0);  // z轴蓝色
    cubeAxesActor->GetLabelTextProperty(2)->SetColor(0.0, 0.0, 1.0);

    cubeAxesActor->SetXTitle("X");
    cubeAxesActor->SetYTitle("Y");
    cubeAxesActor->SetZTitle("Z");

    renderer->AddActor(cubeAxesActor);
}

//  设置视角
void Qtrobtrace_show::setupCameraView()
{
    camera = renderer->GetActiveCamera();
    camera->SetViewUp(0, 0, 1);
    camera->SetPosition(-1000.0, -1000.0, 1000.0);
    camera->SetFocalPoint(0, 0, 0);
    renderer->ResetCamera();
    renderer->ResetCameraClippingRange();
    //renderer->Render();
}

void Qtrobtrace_show::initControlDock()
{
    ui.control_dockWidget->hide();
    controljoint_sliders.push_back(ui.j1);
    controljoint_sliders.push_back(ui.j2);
    controljoint_sliders.push_back(ui.j3);
    controljoint_sliders.push_back(ui.j4);
    controljoint_sliders.push_back(ui.j5);
    controljoint_sliders.push_back(ui.j6);
    // 填充 Label 容器（顺序必须与 Slider 一一对应）
    controljoint_labels = { ui.label_j1, 
                            ui.label_j2, 
                            ui.label_j3, 
                            ui.label_j4, 
                            ui.label_j5, 
                            ui.label_j6 };


    for (int i = 0; i < 6; ++i) {
        connect(controljoint_sliders[i], &QSlider::valueChanged,
            this, [this, i](int value) {
                controljoint_labels[i]->setText(QString::number(value)+"°");
                onControlSliderValueChanged(i, static_cast<double>(value));
            });
    }
}


//增量更新轨迹 (带降采样)
void Qtrobtrace_show::updateTrajectory() {    

    size_t currentIdx = rtde->getCurrentMaxIndex();
    size_t targetIdx = currentIdx;

    if (currentIdx <= m_lastDrawnIndex) {
        return; // 没有新数据
    }
    // 计算目标索引: 向下取整到采样步长的倍数
    
    if (m_sampleStep > 1) {
        targetIdx = (currentIdx / m_sampleStep) * m_sampleStep;
        if (targetIdx <= m_lastDrawnIndex) {
            return; // 还没积累够一个步长
        }
        //qDebug() <<"sampleStep:" << m_sampleStep;
    }
    // 获取增量数据
    //std::vector<std::vector<double>> newPoints;
    //int count = rtde->getTrajectorySegment(m_lastDrawnIndex, targetIdx, newPoints);
    
    vector<RobotData> newPoints;
    int count = rtde->getRobotDataSegment(m_lastDrawnIndex + 1, targetIdx, newPoints);
    
    if (count <= 0) {
        // 可能是初始化时对齐问题，重置索引
        m_lastDrawnIndex = currentIdx;
        return;
    }
    bool boundsChanged = false; // 标记本次是否有新点扩大了范围
    // 追加到VTK
    vtkIdType startId = m_trajPoints->GetNumberOfPoints();
    
    //for (const auto& pose : newPoints.size()) {
    for (size_t i = 0; i < newPoints.size(); ++i) {
        const RobotData& data = newPoints[i];
        if (data.tcpPose.size() >= 3) {
            double x = data.tcpPose[0] * 1000.0;
            double y = data.tcpPose[1] * 1000.0;
            double z = data.tcpPose[2] * 1000.0;
            //qDebug() << "newPoints:" << x << y << z;
            m_trajPoints->InsertNextPoint(x, y, z);
            if (!m_boundsInitialized) {
                // 第一次收到数据：以当前点为基准初始化，但保留 setupCubeAxesActor 里设的默认大范围的“底线”
                m_currentBounds[0] = x; m_currentBounds[1] = x;
                m_currentBounds[2] = y; m_currentBounds[3] = y;
                m_currentBounds[4] = z; m_currentBounds[5] = z;
                m_boundsInitialized = true;
                boundsChanged = true;
            }
            else {
                // 检查是否超出当前记录的边界
                bool expanded = false;
                double margin = 50.0; // 留一点边距 (mm)，避免点正好贴在网格线上
                if (x < m_currentBounds[0]) { m_currentBounds[0] = x - margin; expanded = true; }
                if (x > m_currentBounds[1]) { m_currentBounds[1] = x + margin; expanded = true; }
                if (y < m_currentBounds[2]) { m_currentBounds[2] = y - margin; expanded = true; }
                if (y > m_currentBounds[3]) { m_currentBounds[3] = y + margin; expanded = true; }
                if (z < m_currentBounds[4]) { m_currentBounds[4] = z - margin; expanded = true; }
                if (z > m_currentBounds[5]) { m_currentBounds[5] = z + margin; expanded = true; }
    
                if (expanded) {
                    boundsChanged = true;
                }
            }
        }
        // 更新VTK机器人姿态
        if (data.joints.size() >= 6) {
            moveRobotJoints(
                data.joints[0] * 180.0 / M_PI,
                data.joints[1] * 180.0 / M_PI,
                data.joints[2] * 180.0 / M_PI,
                data.joints[3] * 180.0 / M_PI,
                data.joints[4] * 180.0 / M_PI,
                data.joints[5] * 180.0 / M_PI
            );
        }
        if (!newPoints.empty())
        {
            plot->addDataPoints(newPoints);   // 批量追加，只加数据不重绘
        }
    }
    
    // 重建线条
    rebuildTrajectoryLines();
    
    // 通知VTK更新
    m_trajPoints->Modified();
    m_trajLines->Modified();
    m_trajData->Modified();
    //  检查是否更新
    if (boundsChanged) {
        updateCubeAxesBounds();
    }
    // 更新游标
    m_lastDrawnIndex = targetIdx;

    // 如果高亮球可见，同步更新其视觉大小
    if (highlightActor && highlightActor->GetVisibility()) {
        updateHighlightSize();
    }
         // 渲染
    renderWindow->Render();
}


void Qtrobtrace_show::updateCubeAxesBounds()
{
    if (!cubeAxesActor || !camera) return;
    // 计算新范围的体积或对角线长度
    double diag = sqrt(pow(m_currentBounds[1] - m_currentBounds[0], 2) +
        pow(m_currentBounds[3] - m_currentBounds[2], 2) +
        pow(m_currentBounds[5] - m_currentBounds[4], 2));

    // 记录上一次的范围大小 (需要在类里加一个成员 m_lastDiag)
    bool needReset = false;

    if (m_lastDiag == 0) {
        needReset = true; // 第一次
    }
    else if (diag > m_lastDiag * 1.5) {
        // 如果当前范围比上次大了 50%，说明轨迹大幅扩展，相机可能需要后退
        needReset = true;
    }
    else if (diag < m_lastDiag * 0.5 && m_lastDiag > 100) {
        // 如果范围突然变小（比如重新开始了），相机可能需要拉近
        // 但这种情况较少，通常轨迹是只增不减的
        needReset = true;
    }

    // 更新坐标轴
    cubeAxesActor->SetBounds(m_currentBounds);
    cubeAxesActor->Modified();

    if (needReset) {
        renderer->ResetCamera();
        m_lastDiag = diag; // 更新记录
    }
}

void Qtrobtrace_show::QloadRobotModel()
{
    // 1.导入模型GLB文件
    vtkSmartPointer<vtkGLTFImporter> importer = vtkSmartPointer<vtkGLTFImporter>::New();
    importer->SetFileName("./URmodel.glb");
    importer->SetRenderWindow(renderWindow);
    importer->Update();  

    qDebug() << "机器人模型加载完成";
    vtkActorCollection* actors = renderer->GetActors();
    std::vector<vtkSmartPointer<vtkActor>> actorList;
    actors->InitTraversal();
    // 2.查找关节Actor
    for (int i = 0; i < actors->GetNumberOfItems(); ++i)
    {
        //qDebug() << "Found Actor index:" << i;
        actorList.push_back(actors->GetNextActor());
    }
    for (size_t i = 3; i < actorList.size(); ++i)
    {
        renderer->RemoveActor(actorList[i]);
    }
    // 3. 实例化各个关节的 Assembly (容器)
    // private 私有数据
    // 4. 将对应的 Actor 装入各自的关节容器中
    baseAssembly->AddPart(actorList[3]);
    baseAssembly->AddPart(actorList[4]);
    
    joint1->AddPart(actorList[5]);
    joint1->AddPart(actorList[6]);
    joint1->AddPart(actorList[7]);

    joint2->AddPart(actorList[8]);
    joint2->AddPart(actorList[9]);
    joint2->AddPart(actorList[10]);
    joint2->AddPart(actorList[11]);

    joint3->AddPart(actorList[12]);
    joint3->AddPart(actorList[13]); 
    joint3->AddPart(actorList[14]);

    joint4->AddPart(actorList[15]);
    joint4->AddPart(actorList[16]);
    joint4->AddPart(actorList[17]);

    joint5->AddPart(actorList[18]);
    joint5->AddPart(actorList[19]);
    joint5->AddPart(actorList[20]);

    joint6->AddPart(actorList[21]);
    joint6->AddPart(actorList[22]);

    // 5. 设置每个关节的旋转中心
    baseAssembly->SetOrigin(0.0, 0.0, 0.0);       
    joint1->SetOrigin(0.0, 0.0, 0.0);           
    joint2->SetOrigin(0, 0, 86);          
    joint3->SetOrigin(425,0, 86);
    joint4->SetOrigin(817,0,86);
    joint5->SetOrigin(817,109,0);
    joint6->SetOrigin(817,0,-6);
    //highlightPoint(817, -200,86, 10);

    // 6. 建立父子层级树 (Kinematic Chain)
    baseAssembly->AddPart(joint1); // 1轴装在基座上
    joint1->AddPart(joint2);       // 2轴装在1轴上
    joint2->AddPart(joint3);       // 3轴装在2轴上
    joint3->AddPart(joint4);
    joint4->AddPart(joint5);
    joint5->AddPart(joint6);

    baseAssembly->RotateZ(180);

    // 7. 将最顶层的“根节点”重新添加到渲染器中
    renderer->AddActor(baseAssembly);
    // 8.运动学控制

    joint2->RotateY(-90);
    joint4->RotateY(-90);
    
    //--- 至此，层级树构建完毕 ---
    renderer->ResetCamera();
    renderWindow->Render();

}

void Qtrobtrace_show::highlightPoint(double x, double y, double z, double radius)
{
    // 1. 创建球体（覆盖在目标点）
    vtkNew<vtkSphereSource> sphere;
    sphere->SetCenter(x, y, z);
    sphere->SetRadius(radius); // 根据模型大小调整（机械臂建议0.005~0.02）
    sphere->SetPhiResolution(20);
    sphere->SetThetaResolution(20);

    // 2. 创建Actor并设置高亮样式
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(sphere->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(1.0, 0.0, 0.0); // 红色（醒目）
    actor->GetProperty()->SetOpacity(0.7);         // 半透明（不遮挡模型）
    actor->GetProperty()->SetSpecular(0.5);        // 增加光泽感

    // 3. 添加到渲染器（自动管理生命周期）
    renderer->AddActor(actor);
    renderWindow->Render();

    // 【可选】保存引用以便后续移除
    // m_highlightActors.append(actor); 
    qDebug() << "高亮点已添加:" << QString("(%1, %2, %3)").arg(x).arg(y).arg(z);
}

void Qtrobtrace_show::moveRobotJoints(double angle1, double angle2, double angle3, double angle4, double angle5, double angle6)
{
    joint1->SetOrientation(0, 0, angle1); 
    joint2->SetOrientation(0, angle2, 0);
    joint3->SetOrientation(0, angle3, 0);
    joint4->SetOrientation(0, angle4, 0);
    joint5->SetOrientation(0, 0, -angle5);
    //joint6->SetOrientation(0, 0,angle6); 
    // 统一执行渲染
    this->renderWindow->Render();
}

void Qtrobtrace_show::rebuildTrajectoryLines()
{
    m_trajLines->Reset();
    vtkIdType totalPoints = m_trajPoints->GetNumberOfPoints();
    if (totalPoints >= 2) {
        vtkSmartPointer<vtkPolyLine> line = vtkSmartPointer<vtkPolyLine>::New();
        line->GetPointIds()->SetNumberOfIds(totalPoints);
        for (vtkIdType i = 0; i < totalPoints; ++i) {
            line->GetPointIds()->SetId(i, i);
        }
        m_trajLines->InsertNextCell(line);
    }
}

void Qtrobtrace_show::onPlaybackSliderChanged(int value)
{
    size_t idx = static_cast<size_t>(value);    
    RobotData data;
    if (m_isUsingLoadedData && idx < m_loadedDataCache.size()) {
        // 如果当前使用的是导入的数据
        data = m_loadedDataCache[idx];
    }
    else if (!m_isUsingLoadedData && rtde) {
        // 如果当前使用的是RTDE实时数据
        data = rtde->getDataByIndex(idx);
    }
    else {
        // 索引越界或其他错误情况
        qDebug() << "Error: Index out of bounds or no data source available.";
        return;
    }
    // 1. 更新 2D 图表游标
    plot->setCurrentIndex(idx);
    // 2. 更新 VTK 机器人模型姿态
    if (data.joints.size() >= 6) {
        moveRobotJoints(
            data.joints[0] * 180.0 / M_PI,
            data.joints[1] * 180.0 / M_PI,
            data.joints[2] * 180.0 / M_PI,
            data.joints[3] * 180.0 / M_PI,
            data.joints[4] * 180.0 / M_PI,
            data.joints[5] * 180.0 / M_PI
        );
    }
    renderWindow->Render();
}

void Qtrobtrace_show::onControlSliderValueChanged(int jointIndex, double angleDeg)
{ 
    targetJointAngles[jointIndex] = angleDeg;

    //qDebug() << targetJointAngles;
    // 调用 RTDE 控制接口 
    // 直接调用 RTDE 的异步控制接口
    if (rtde && rtde->isConnected()) {
        // 传入当前所有关节的目标值
        rtde->moveJ_Async(targetJointAngles, 0.5, 0.5); // 速度和加速度可根据需要调整
    }

    // 更新3D模型
    moveRobotJoints(
        targetJointAngles[0], targetJointAngles[1], targetJointAngles[2],
        targetJointAngles[3], targetJointAngles[4], targetJointAngles[5]
    );

}


void Qtrobtrace_show::dataAnalysis_clicked()
{
    // 获取当前要分析的数据（可以是导入的离线数据，也可以是刚采集的在线数据）
    std::vector<RobotData> dataToAnalyze;
    if (m_isUsingLoadedData && !m_loadedDataCache.empty()) {
        dataToAnalyze = m_loadedDataCache;
    }
    else if (rtde) {
        size_t maxIdx = rtde->getCurrentMaxIndex();
        rtde->getRobotDataSegment(0, maxIdx, dataToAnalyze);
    }

    if (dataToAnalyze.size() < 2) {
        QMessageBox::information(this, "提示", "数据点不足，无法进行分析！");
        return;
    }

    double totalLength = 0.0;     // 轨迹总长度 (mm)
    double maxSpeed = 0.0;        // 最大速度 (mm/s)
    double avgSpeed = 0.0;        // 平均速度 (mm/s)

    // 遍历数据点，进行差分计算
    for (size_t i = 1; i < dataToAnalyze.size(); ++i) {
        // 1. 计算欧氏距离 (Δx, Δy, Δz)
        double dx = (dataToAnalyze[i].tcpPose[0] - dataToAnalyze[i - 1].tcpPose[0]) * 1000.0;
        double dy = (dataToAnalyze[i].tcpPose[1] - dataToAnalyze[i - 1].tcpPose[1]) * 1000.0;
        double dz = (dataToAnalyze[i].tcpPose[2] - dataToAnalyze[i - 1].tcpPose[2]) * 1000.0;
        double dist = std::sqrt(dx * dx + dy * dy + dz * dz);
        totalLength += dist;

        // 2. 计算速度 (v = s / t)
        double dt = (dataToAnalyze[i].timestampMs - dataToAnalyze[i - 1].timestampMs) / 1000.0; // 转换为秒
        if (dt > 0) {
            double speed = dist / dt;
            if (speed > maxSpeed) {
                maxSpeed = speed;
            }
        }
    }

    // 计算平均速度
    double totalTime = (dataToAnalyze.back().timestampMs - dataToAnalyze.front().timestampMs) / 1000.0;
    if (totalTime > 0) {
        avgSpeed = totalLength / totalTime;
    }

    // 生成报告字符串
    QString report = QString("==== 运动轨迹数据分析报告 ====\n\n")
        + QString("总数据点数： %1 个\n").arg(dataToAnalyze.size())
        + QString("总运行时长： %1 秒\n").arg(totalTime, 0, 'f', 2)
        + QString("------------------------------\n")
        + QString("轨迹总长度： %1 mm\n").arg(totalLength, 0, 'f', 2)
        + QString("平均末端速度： %1 mm/s\n").arg(avgSpeed, 0, 'f', 2)
        + QString("最大末端速度： %1 mm/s\n").arg(maxSpeed, 0, 'f', 2);

    // 弹窗展示结果
    QMessageBox::information(this, "数据分析报告", report);
}