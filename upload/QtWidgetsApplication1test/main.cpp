
#include "stdafx.h"
#include "robtrace_show.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
    // 分配控制台
    AllocConsole();

    // 重定向 stdout/stderr 到控制台
    FILE* f = nullptr;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONIN$", "r", stdin);

    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    QApplication app(argc, argv);
    // 1. 创建启动画面
    QPixmap pixmap("./startpic.png");
    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents(); // 立即刷新显示
    QTimer::singleShot(1000, &splash, [&]() {
        splash.finish(nullptr); // 关闭启动画面
        });

    Qtrobtrace_show window;
    
    window.setWindowTitle("RobTrace");
    QIcon icon("./图标.ico");
    window.setWindowIcon(icon);

    window.show();
    return app.exec();
}
