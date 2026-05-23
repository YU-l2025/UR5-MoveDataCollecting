/********************************************************************************
** Form generated from reading UI file 'robtrace_show.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROBTRACE_SHOW_H
#define UI_ROBTRACE_SHOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKOpenGLNativeWidget.h"
#include "customshow.h"

QT_BEGIN_NAMESPACE

class Ui_Qtrobtrace_show
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QVTKOpenGLNativeWidget *VTKwidget;
    QDockWidget *control_dockWidget;
    QWidget *dockWidgetContents;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_j1;
    QSlider *j1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_j2;
    QSlider *j2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_j3;
    QSlider *j3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_j4;
    QSlider *j4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_j5;
    QSlider *j5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_j6;
    QSlider *j6;
    QSlider *TimeSlider;
    CustomShow *customwidget;
    QWidget *widget;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QPushButton *importfile;
    QPushButton *savefile;
    QPushButton *fulltrace;
    QPushButton *data_analysis;
    QPushButton *controlbar;
    QWidget *widget_2;
    QLineEdit *IP;
    QPushButton *connect_button;
    QPushButton *disconnect_button;
    QLabel *state_label;
    QPushButton *collect_button;

    void setupUi(QWidget *Qtrobtrace_show)
    {
        if (Qtrobtrace_show->objectName().isEmpty())
            Qtrobtrace_show->setObjectName(QString::fromUtf8("Qtrobtrace_show"));
        Qtrobtrace_show->resize(1376, 987);
        gridLayout = new QGridLayout(Qtrobtrace_show);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        VTKwidget = new QVTKOpenGLNativeWidget(Qtrobtrace_show);
        VTKwidget->setObjectName(QString::fromUtf8("VTKwidget"));
        VTKwidget->setMinimumSize(QSize(1200, 700));
        VTKwidget->setStyleSheet(QString::fromUtf8(""));
        control_dockWidget = new QDockWidget(VTKwidget);
        control_dockWidget->setObjectName(QString::fromUtf8("control_dockWidget"));
        control_dockWidget->setGeometry(QRect(850, 430, 351, 261));
        control_dockWidget->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        layoutWidget = new QWidget(dockWidgetContents);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 341, 221));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_j1 = new QLabel(layoutWidget);
        label_j1->setObjectName(QString::fromUtf8("label_j1"));
        label_j1->setMinimumSize(QSize(20, 0));
        label_j1->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_j1);

        j1 = new QSlider(layoutWidget);
        j1->setObjectName(QString::fromUtf8("j1"));
        j1->setMinimum(-180);
        j1->setMaximum(180);
        j1->setOrientation(Qt::Horizontal);
        j1->setTickPosition(QSlider::TicksAbove);
        j1->setTickInterval(30);

        horizontalLayout->addWidget(j1);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_j2 = new QLabel(layoutWidget);
        label_j2->setObjectName(QString::fromUtf8("label_j2"));
        label_j2->setMinimumSize(QSize(20, 0));
        label_j2->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_j2);

        j2 = new QSlider(layoutWidget);
        j2->setObjectName(QString::fromUtf8("j2"));
        j2->setMinimum(-180);
        j2->setMaximum(180);
        j2->setValue(-90);
        j2->setOrientation(Qt::Horizontal);
        j2->setTickPosition(QSlider::TicksAbove);
        j2->setTickInterval(30);

        horizontalLayout_2->addWidget(j2);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_j3 = new QLabel(layoutWidget);
        label_j3->setObjectName(QString::fromUtf8("label_j3"));
        label_j3->setMinimumSize(QSize(20, 0));
        label_j3->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_j3);

        j3 = new QSlider(layoutWidget);
        j3->setObjectName(QString::fromUtf8("j3"));
        j3->setMinimum(-180);
        j3->setMaximum(180);
        j3->setOrientation(Qt::Horizontal);
        j3->setTickPosition(QSlider::TicksAbove);
        j3->setTickInterval(30);

        horizontalLayout_3->addWidget(j3);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_j4 = new QLabel(layoutWidget);
        label_j4->setObjectName(QString::fromUtf8("label_j4"));
        label_j4->setMinimumSize(QSize(20, 0));
        label_j4->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_j4);

        j4 = new QSlider(layoutWidget);
        j4->setObjectName(QString::fromUtf8("j4"));
        j4->setMinimum(-180);
        j4->setMaximum(180);
        j4->setValue(-90);
        j4->setOrientation(Qt::Horizontal);
        j4->setTickPosition(QSlider::TicksAbove);
        j4->setTickInterval(30);

        horizontalLayout_4->addWidget(j4);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_j5 = new QLabel(layoutWidget);
        label_j5->setObjectName(QString::fromUtf8("label_j5"));
        label_j5->setMinimumSize(QSize(20, 0));
        label_j5->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_j5);

        j5 = new QSlider(layoutWidget);
        j5->setObjectName(QString::fromUtf8("j5"));
        j5->setMinimum(-180);
        j5->setMaximum(180);
        j5->setValue(0);
        j5->setSliderPosition(0);
        j5->setOrientation(Qt::Horizontal);
        j5->setTickPosition(QSlider::TicksAbove);
        j5->setTickInterval(30);

        horizontalLayout_5->addWidget(j5);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_j6 = new QLabel(layoutWidget);
        label_j6->setObjectName(QString::fromUtf8("label_j6"));
        label_j6->setMinimumSize(QSize(20, 0));
        label_j6->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(label_j6);

        j6 = new QSlider(layoutWidget);
        j6->setObjectName(QString::fromUtf8("j6"));
        j6->setMinimum(-180);
        j6->setMaximum(180);
        j6->setOrientation(Qt::Horizontal);
        j6->setTickPosition(QSlider::TicksAbove);
        j6->setTickInterval(30);

        horizontalLayout_6->addWidget(j6);


        verticalLayout_3->addLayout(horizontalLayout_6);

        control_dockWidget->setWidget(dockWidgetContents);

        verticalLayout_2->addWidget(VTKwidget);

        TimeSlider = new QSlider(Qtrobtrace_show);
        TimeSlider->setObjectName(QString::fromUtf8("TimeSlider"));
        TimeSlider->setMinimumSize(QSize(1000, 0));
        TimeSlider->setMaximumSize(QSize(16777215, 16777215));
        TimeSlider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(TimeSlider);

        customwidget = new CustomShow(Qtrobtrace_show);
        customwidget->setObjectName(QString::fromUtf8("customwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(customwidget->sizePolicy().hasHeightForWidth());
        customwidget->setSizePolicy(sizePolicy);
        customwidget->setMinimumSize(QSize(1000, 230));
        customwidget->setMaximumSize(QSize(16777215, 350));
        customwidget->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_2->addWidget(customwidget);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 1, 1);

        widget = new QWidget(Qtrobtrace_show);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        widget->setMinimumSize(QSize(150, 0));
        widget->setMaximumSize(QSize(150, 16777215));
        layoutWidget1 = new QWidget(widget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 150, 151, 161));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        importfile = new QPushButton(layoutWidget1);
        importfile->setObjectName(QString::fromUtf8("importfile"));
        importfile->setMinimumSize(QSize(130, 0));
        importfile->setMaximumSize(QSize(150, 16777215));

        verticalLayout->addWidget(importfile);

        savefile = new QPushButton(layoutWidget1);
        savefile->setObjectName(QString::fromUtf8("savefile"));
        savefile->setMinimumSize(QSize(130, 0));
        savefile->setMaximumSize(QSize(150, 16777215));

        verticalLayout->addWidget(savefile);

        fulltrace = new QPushButton(layoutWidget1);
        fulltrace->setObjectName(QString::fromUtf8("fulltrace"));
        fulltrace->setMinimumSize(QSize(130, 0));
        fulltrace->setMaximumSize(QSize(150, 16777215));

        verticalLayout->addWidget(fulltrace);

        data_analysis = new QPushButton(layoutWidget1);
        data_analysis->setObjectName(QString::fromUtf8("data_analysis"));
        data_analysis->setMinimumSize(QSize(130, 0));
        data_analysis->setMaximumSize(QSize(150, 16777215));

        verticalLayout->addWidget(data_analysis);

        controlbar = new QPushButton(layoutWidget1);
        controlbar->setObjectName(QString::fromUtf8("controlbar"));
        controlbar->setMinimumSize(QSize(130, 0));
        controlbar->setMaximumSize(QSize(150, 16777215));

        verticalLayout->addWidget(controlbar);

        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(10, 10, 131, 111));
        IP = new QLineEdit(widget_2);
        IP->setObjectName(QString::fromUtf8("IP"));
        IP->setGeometry(QRect(0, 0, 131, 21));
        connect_button = new QPushButton(widget_2);
        connect_button->setObjectName(QString::fromUtf8("connect_button"));
        connect_button->setGeometry(QRect(0, 50, 61, 31));
        disconnect_button = new QPushButton(widget_2);
        disconnect_button->setObjectName(QString::fromUtf8("disconnect_button"));
        disconnect_button->setGeometry(QRect(80, 50, 51, 31));
        state_label = new QLabel(widget_2);
        state_label->setObjectName(QString::fromUtf8("state_label"));
        state_label->setGeometry(QRect(10, 20, 71, 31));
        collect_button = new QPushButton(widget_2);
        collect_button->setObjectName(QString::fromUtf8("collect_button"));
        collect_button->setGeometry(QRect(0, 80, 131, 31));

        gridLayout->addWidget(widget, 0, 0, 1, 1);


        retranslateUi(Qtrobtrace_show);
        QObject::connect(importfile, SIGNAL(clicked()), Qtrobtrace_show, SLOT(importfile_clicked()));
        QObject::connect(fulltrace, SIGNAL(clicked()), Qtrobtrace_show, SLOT(fulltrace_clicked()));
        QObject::connect(controlbar, SIGNAL(clicked()), Qtrobtrace_show, SLOT(indicator_clicked()));
        QObject::connect(connect_button, SIGNAL(clicked()), Qtrobtrace_show, SLOT(connect_clicked()));
        QObject::connect(collect_button, SIGNAL(clicked()), Qtrobtrace_show, SLOT(collect_clicked()));
        QObject::connect(savefile, SIGNAL(clicked()), savefile, SLOT(click()));
        QObject::connect(savefile, SIGNAL(clicked()), Qtrobtrace_show, SLOT(savefile_clicked()));
        QObject::connect(controlbar, SIGNAL(clicked()), Qtrobtrace_show, SLOT(control_clicked()));
        QObject::connect(disconnect_button, SIGNAL(clicked()), Qtrobtrace_show, SLOT(disconnect_clicked()));
        QObject::connect(data_analysis, SIGNAL(clicked()), Qtrobtrace_show, SLOT(dataAnalysis_clicked()));

        QMetaObject::connectSlotsByName(Qtrobtrace_show);
    } // setupUi

    void retranslateUi(QWidget *Qtrobtrace_show)
    {
        Qtrobtrace_show->setWindowTitle(QApplication::translate("Qtrobtrace_show", "robtrace_show", nullptr));
        label_j1->setText(QApplication::translate("Qtrobtrace_show", "0\302\260", nullptr));
        label_j2->setText(QApplication::translate("Qtrobtrace_show", "-90\302\260", nullptr));
        label_j3->setText(QApplication::translate("Qtrobtrace_show", "0\302\260", nullptr));
        label_j4->setText(QApplication::translate("Qtrobtrace_show", "-90\302\260", nullptr));
        label_j5->setText(QApplication::translate("Qtrobtrace_show", "0\302\260", nullptr));
        label_j6->setText(QApplication::translate("Qtrobtrace_show", "0\302\260", nullptr));
        importfile->setText(QApplication::translate("Qtrobtrace_show", "\345\257\274\345\205\245\346\226\207\344\273\266", nullptr));
        savefile->setText(QApplication::translate("Qtrobtrace_show", "\344\277\235\345\255\230\346\226\207\344\273\266", nullptr));
        fulltrace->setText(QApplication::translate("Qtrobtrace_show", "\346\225\264\344\275\223\345\233\276\345\203\217", nullptr));
        data_analysis->setText(QApplication::translate("Qtrobtrace_show", "\346\225\260\346\215\256\345\210\206\346\236\220", nullptr));
        controlbar->setText(QApplication::translate("Qtrobtrace_show", "\346\216\247\345\210\266\345\231\250", nullptr));
        connect_button->setText(QApplication::translate("Qtrobtrace_show", "\350\277\236\346\216\245", nullptr));
        disconnect_button->setText(QApplication::translate("Qtrobtrace_show", "\346\226\255\345\274\200", nullptr));
        state_label->setText(QApplication::translate("Qtrobtrace_show", "\346\234\252\350\277\236\346\216\245", nullptr));
        collect_button->setText(QApplication::translate("Qtrobtrace_show", "\345\274\200\345\247\213\351\207\207\351\233\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Qtrobtrace_show: public Ui_Qtrobtrace_show {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROBTRACE_SHOW_H
