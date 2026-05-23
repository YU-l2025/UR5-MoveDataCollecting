#pragma once

#include <QtWidgets/QWidget>
#include "ui_robtrace_show.h"

#include <vtkPlatform.h>
#include <vtkCommonCoreModule.h>
#include <vtkpoints.h>
#include <vtkSmartPointer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkNamedColors.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkProperty.h>
#include <vtkCubeAxesActor.h>
#include <vtkTextProperty.h>
#include <vtkPointLocator.h>
#include <vtkCellPicker.h>
#include <vtkBillboardTextActor3D.h>
#include <vtkSphereSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkKdTree.h>
#include <vtkCamera.h>
#include <vtkTransform.h> 
#include <vtkCaptionActor2D.h>
#include <vtkCollection.h> 
#include <vtkOBJReader.h>
#include <vtkOBJImporter.h>
#include <vtkAssembly.h>
#include <vtkGLTFImporter.h>

#include "RTDE.h"
#include "customshow.h"
#include "datafile.h"
#include "data_process.h"


class QVTKOpenGLNativeWidget;
class vtkGenericOpenGLRenderWindow;

class Qtrobtrace_show : public QWidget
{
    Q_OBJECT

public:
    Qtrobtrace_show(QWidget *parent = nullptr);
    ~Qtrobtrace_show();

    // 设置采样步长 (500Hz -> 50Hz = 10)
    void setSampleStep(int step) { m_sampleStep = step; }
private:
    Ui::Qtrobtrace_show ui;

    QVTKOpenGLNativeWidget* vtkWin;

    vtkGenericOpenGLRenderWindow* renderWindow;

    vtkSmartPointer<vtkAxesActor> axesActor;
    
    vtkSmartPointer<vtkOrientationMarkerWidget> orientationWidget;

    vtkSmartPointer<vtkCubeAxesActor> cubeAxesActor;

    vtkSmartPointer<vtkPolyData> polyData;          // 曲线数据
    
    vtkSmartPointer<vtkActor> actor;                // 曲线 actor

    vtkSmartPointer<vtkCellPicker> cellPicker;

    vtkSmartPointer<vtkSphereSource> highlightSphere;
    vtkSmartPointer<vtkPolyDataMapper> highlightMapper;
    vtkSmartPointer<vtkActor> highlightActor;
    double m_desiredScreenDiameter; // 期望屏幕直径（像素）
    void updateHighlightSize();     // 声明更新函数
    vtkSmartPointer<vtkBillboardTextActor3D> coordTextActor;  // 3D 坐标文本

    vtkCamera* camera;

    // 位姿指示器 actor
    vtkSmartPointer<vtkAxesActor> poseActor;
    vtkSmartPointer<vtkRenderer> renderer;

    CustomShow* plot;
    QTimer* m_plotUpdateTimer = nullptr;


    //  RTDE数据传输
    RTDE* rtde = nullptr;

    QTimer * m_renderTimer;      // 控制渲染频率
    size_t m_lastDrawnIndex;    // 上次画到的索引
    int m_sampleStep;           // 采样步长 (默认10)
    vtkSmartPointer<vtkPoints> m_trajPoints;   // 轨迹点
    vtkSmartPointer<vtkCellArray> m_trajLines; // 轨迹线
    vtkSmartPointer<vtkPolyData> m_trajData;
    vtkSmartPointer<vtkPolyDataMapper> m_trajMapper;
    vtkSmartPointer<vtkActor> m_trajActor;

    // 用于记录当前场景的有效范围，避免每次全量计算
    double m_currentBounds[6];
    bool m_boundsInitialized; // 标记是否已经初始化过范围
    bool m_cameraAutoAdjusted;//    相机位置
    double m_lastDiag = 0.0;

    //  机器人关节实例
    vtkSmartPointer<vtkAssembly> baseAssembly = vtkSmartPointer<vtkAssembly>::New();
    vtkSmartPointer<vtkAssembly> joint1 = vtkSmartPointer<vtkAssembly>::New();
    vtkSmartPointer<vtkAssembly> joint2 = vtkSmartPointer<vtkAssembly>::New();
    vtkSmartPointer<vtkAssembly> joint3 = vtkSmartPointer<vtkAssembly>::New();
    vtkSmartPointer<vtkAssembly> joint4 = vtkSmartPointer<vtkAssembly>::New();
    vtkSmartPointer<vtkAssembly> joint5 = vtkSmartPointer<vtkAssembly>::New();
    vtkSmartPointer<vtkAssembly> joint6 = vtkSmartPointer<vtkAssembly>::New();
    
    //导入数据缓冲区
    std::vector<RobotData> m_loadedDataCache;

    bool m_isUsingLoadedData = false; // 标志：当前是否在播放导入的数据
    // 滑动条控制相关
    size_t m_currentPlayIndex = 0;
    bool m_isPlaying = false;
    std::vector<QSlider*> controljoint_sliders;
    std::vector<double> targetJointAngles = { 0.0, -90.0, 0.0, -90.0, 0.0, 0.0 };
    std::vector<QLabel*> controljoint_labels; // 存放显示的 label

    void initVtkWindowAndRenderer();
    void createTrajectoryCurve();
    void createHighlightAndText();
    void setupCustomInteractorStyle();
    void setupOrientationMarker();
    void setupCubeAxesActor();
    void setupCameraView();

    void collectdata();

    void updateTrajectory();
    // 增量更新轨迹
    // 动态更新坐标轴范围的函数
    void updateCubeAxesBounds();

    //  加载模型
    void QloadRobotModel();

    void highlightPoint(double x, double y, double z, double radius);   
    //  控制运动
    void moveRobotJoints(double angle1, double angle2, double angle3, double angle4, double angle5, double angle6);

    void initTimer();
    void initControlDock();
    //void updateControlDock(int jointIndex, double angleDeg);

private slots:
    void importfile_clicked();
    void fulltrace_clicked();
    void control_clicked();
    void connect_clicked();
    void collect_clicked();
    void savefile_clicked();
    void disconnect_clicked();
    void dataAnalysis_clicked();

    // 重建轨迹线
    void rebuildTrajectoryLines();
    void onPlaybackSliderChanged(int value);
    //  关节控制dock
    void onControlSliderValueChanged(int jointIndex, double angleDeg);

signals:
    void start_collect();
    void stop_collect();
};


// 自定义交互风格
class CustomStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static CustomStyle* New();
    vtkTypeMacro(CustomStyle, vtkInteractorStyleTrackballCamera);

    void SetRenderer(vtkRenderer* r) { renderer = r; }
    // 设置数据的同时构建定位器，解决卡顿的核心
    void SetPolyData(vtkPolyData* pd) {
        polyData = pd;
        if (polyData) {
            locator = vtkSmartPointer<vtkPointLocator>::New();
            locator->SetDataSet(polyData);
            locator->BuildLocator(); // 构建搜索树，一次构建，无限快查
        }
    }
    void SetHighlightSphere(vtkSphereSource* hs) { highlightSphere = hs; }
    void SetHighlightActor(vtkActor* ha) { highlightActor = ha; }
    void SetTextActor(vtkBillboardTextActor3D* ta) { textActor = ta; }  // 改名一致
    void SetCellPicker(vtkCellPicker* cp) { cellPicker = cp; }

    virtual void OnLeftButtonDown() override
    {
        if (!polyData || !polyData->GetPoints()) { // 【新增】检查数据是否有效
            vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
            return;
        }

        int* pos = GetInteractor()->GetEventPosition();
        cellPicker->Pick(pos[0], pos[1], 0.0, renderer);

        if (cellPicker->GetCellId() != -1)
        {
            // 拾取到了线条上的某处
            double pickPos[3];
            cellPicker->GetPickPosition(pickPos);

            // 【优化】使用 Locator 瞬间找到最近点的 ID
            vtkIdType closestPointId = locator->FindClosestPoint(pickPos);

            if (closestPointId != -1 && closestPointId < polyData->GetNumberOfPoints())
            {
                double selectedPt[3];
                polyData->GetPoint(closestPointId, selectedPt);

                // 高亮球移到选中点
                //highlightSphere->SetCenter(selectedPt);
                //highlightSphere->Update();
                //highlightActor->SetVisibility(1);
                highlightActor->SetPosition(selectedPt[0], selectedPt[1], selectedPt[2]);
                highlightActor->SetVisibility(1);
                double dist = renderer->GetActiveCamera()->GetPosition()[0] - selectedPt[0];
                // 实际用下面完整计算
                {
                    vtkCamera* cam = renderer->GetActiveCamera();
                    double camPos[3], dx, dy, dz, d;
                    cam->GetPosition(camPos);
                    dx = camPos[0] - selectedPt[0]; dy = camPos[1] - selectedPt[1]; dz = camPos[2] - selectedPt[2];
                    d = std::sqrt(dx * dx + dy * dy + dz * dz);
                    if (d > 1e-5) {
                        int* sz = this->GetInteractor()->GetRenderWindow()->GetSize();
                        double h = sz[1] > 0 ? sz[1] : 600.0;
                        double r = (15.0 * d * std::tan(vtkMath::RadiansFromDegrees(cam->GetViewAngle()) * 0.5)) / h;
                        highlightActor->SetScale(r, r, r); // ✅ 关键：动态缩放
                    }
                }
                // 3D 文本显示坐标（放在点上方）
      /*          char text[128];
                sprintf(text, "X: %.3f\nY: %.3f\nZ: %.3f", selectedPt[0], selectedPt[1], selectedPt[2]);
                textActor->SetInput(text); */                                     // 改成 textActor
                QString coordStr = QString("X: %1\nY: %2\nZ: %3")
                    .arg(selectedPt[0], 0, 'f', 3)  // %.3f，保留3位小数
                    .arg(selectedPt[1], 0, 'f', 3)
                    .arg(selectedPt[2], 0, 'f', 3);

                textActor->SetInput(coordStr.toUtf8().constData());

                textActor->SetPosition(selectedPt[0], selectedPt[1] + 0.15, selectedPt[2]);
                textActor->SetVisibility(1);
                
                GetInteractor()->GetRenderWindow()->Render();
            }
            else {
                // 无效点：隐藏高亮
                highlightActor->SetVisibility(0);
                textActor->SetVisibility(0);
            }
        }
        else
        {
            // === 没拾取到任何东西（点击了空白处） ===
            // 隐藏高亮和文字
            highlightActor->SetVisibility(0);
            textActor->SetVisibility(0);
        }

        // 保留默认交互行为
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

    virtual void OnRightButtonDown() override
    {
        // 调用 StartRotate 进入旋转模式
        this->StartRotate();
    }
    virtual void OnRightButtonUp() override
    {
        // 结束旋转
        this->EndRotate();
    }

private:
    vtkRenderer* renderer = nullptr;
    vtkPolyData* polyData = nullptr;
    vtkSphereSource* highlightSphere = nullptr;
    vtkActor* highlightActor = nullptr;
    vtkBillboardTextActor3D* textActor = nullptr;  // 统一用 textActor
    vtkCellPicker* cellPicker = nullptr;
    vtkSmartPointer<vtkKdTree> kdTree;

    // 关键优化成员
    vtkSmartPointer<vtkPointLocator> locator;
};

