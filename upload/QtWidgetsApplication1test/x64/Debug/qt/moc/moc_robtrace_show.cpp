/****************************************************************************
** Meta object code from reading C++ file 'robtrace_show.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.10)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../../../robtrace_show.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'robtrace_show.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.10. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Qtrobtrace_show_t {
    QByteArrayData data[18];
    char stringdata0[287];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Qtrobtrace_show_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Qtrobtrace_show_t qt_meta_stringdata_Qtrobtrace_show = {
    {
QT_MOC_LITERAL(0, 0, 15), // "Qtrobtrace_show"
QT_MOC_LITERAL(1, 16, 13), // "start_collect"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 12), // "stop_collect"
QT_MOC_LITERAL(4, 44, 18), // "importfile_clicked"
QT_MOC_LITERAL(5, 63, 17), // "fulltrace_clicked"
QT_MOC_LITERAL(6, 81, 15), // "control_clicked"
QT_MOC_LITERAL(7, 97, 15), // "connect_clicked"
QT_MOC_LITERAL(8, 113, 15), // "collect_clicked"
QT_MOC_LITERAL(9, 129, 16), // "savefile_clicked"
QT_MOC_LITERAL(10, 146, 18), // "disconnect_clicked"
QT_MOC_LITERAL(11, 165, 20), // "dataAnalysis_clicked"
QT_MOC_LITERAL(12, 186, 22), // "rebuildTrajectoryLines"
QT_MOC_LITERAL(13, 209, 23), // "onPlaybackSliderChanged"
QT_MOC_LITERAL(14, 233, 5), // "value"
QT_MOC_LITERAL(15, 239, 27), // "onControlSliderValueChanged"
QT_MOC_LITERAL(16, 267, 10), // "jointIndex"
QT_MOC_LITERAL(17, 278, 8) // "angleDeg"

    },
    "Qtrobtrace_show\0start_collect\0\0"
    "stop_collect\0importfile_clicked\0"
    "fulltrace_clicked\0control_clicked\0"
    "connect_clicked\0collect_clicked\0"
    "savefile_clicked\0disconnect_clicked\0"
    "dataAnalysis_clicked\0rebuildTrajectoryLines\0"
    "onPlaybackSliderChanged\0value\0"
    "onControlSliderValueChanged\0jointIndex\0"
    "angleDeg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Qtrobtrace_show[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    0,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    0,   87,    2, 0x08 /* Private */,
      11,    0,   88,    2, 0x08 /* Private */,
      12,    0,   89,    2, 0x08 /* Private */,
      13,    1,   90,    2, 0x08 /* Private */,
      15,    2,   93,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int, QMetaType::Double,   16,   17,

       0        // eod
};

void Qtrobtrace_show::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Qtrobtrace_show *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->start_collect(); break;
        case 1: _t->stop_collect(); break;
        case 2: _t->importfile_clicked(); break;
        case 3: _t->fulltrace_clicked(); break;
        case 4: _t->control_clicked(); break;
        case 5: _t->connect_clicked(); break;
        case 6: _t->collect_clicked(); break;
        case 7: _t->savefile_clicked(); break;
        case 8: _t->disconnect_clicked(); break;
        case 9: _t->dataAnalysis_clicked(); break;
        case 10: _t->rebuildTrajectoryLines(); break;
        case 11: _t->onPlaybackSliderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->onControlSliderValueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Qtrobtrace_show::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Qtrobtrace_show::start_collect)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Qtrobtrace_show::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Qtrobtrace_show::stop_collect)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Qtrobtrace_show::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_Qtrobtrace_show.data,
    qt_meta_data_Qtrobtrace_show,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Qtrobtrace_show::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Qtrobtrace_show::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Qtrobtrace_show.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Qtrobtrace_show::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Qtrobtrace_show::start_collect()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Qtrobtrace_show::stop_collect()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
