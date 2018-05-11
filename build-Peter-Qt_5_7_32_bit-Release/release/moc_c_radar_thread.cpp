/****************************************************************************
** Meta object code from reading C++ file 'c_radar_thread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QT/c_radar_thread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'c_radar_thread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_dataProcessingThread_t {
    QByteArrayData data[15];
    char stringdata0[204];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_dataProcessingThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_dataProcessingThread_t qt_meta_stringdata_dataProcessingThread = {
    {
QT_MOC_LITERAL(0, 0, 20), // "dataProcessingThread"
QT_MOC_LITERAL(1, 21, 19), // "HeadingDataReceived"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 7), // "heading"
QT_MOC_LITERAL(4, 50, 14), // "ReadDataBuffer"
QT_MOC_LITERAL(5, 65, 16), // "PushCommandQueue"
QT_MOC_LITERAL(6, 82, 16), // "processRadarData"
QT_MOC_LITERAL(7, 99, 15), // "processARPAData"
QT_MOC_LITERAL(8, 115, 9), // "inputdata"
QT_MOC_LITERAL(9, 125, 17), // "playbackRadarData"
QT_MOC_LITERAL(10, 143, 14), // "SerialDataRead"
QT_MOC_LITERAL(11, 158, 9), // "gpsupdate"
QT_MOC_LITERAL(12, 168, 16), // "QGeoPositionInfo"
QT_MOC_LITERAL(13, 185, 3), // "geo"
QT_MOC_LITERAL(14, 189, 14) // "StopProcessing"

    },
    "dataProcessingThread\0HeadingDataReceived\0"
    "\0heading\0ReadDataBuffer\0PushCommandQueue\0"
    "processRadarData\0processARPAData\0"
    "inputdata\0playbackRadarData\0SerialDataRead\0"
    "gpsupdate\0QGeoPositionInfo\0geo\0"
    "StopProcessing"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_dataProcessingThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   62,    2, 0x08 /* Private */,
       5,    0,   63,    2, 0x08 /* Private */,
       6,    0,   64,    2, 0x08 /* Private */,
       7,    1,   65,    2, 0x08 /* Private */,
       9,    0,   68,    2, 0x08 /* Private */,
      10,    0,   69,    2, 0x08 /* Private */,
      11,    1,   70,    2, 0x08 /* Private */,
      14,    0,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,

       0        // eod
};

void dataProcessingThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        dataProcessingThread *_t = static_cast<dataProcessingThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->HeadingDataReceived((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->ReadDataBuffer(); break;
        case 2: _t->PushCommandQueue(); break;
        case 3: _t->processRadarData(); break;
        case 4: _t->processARPAData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 5: _t->playbackRadarData(); break;
        case 6: _t->SerialDataRead(); break;
        case 7: _t->gpsupdate((*reinterpret_cast< QGeoPositionInfo(*)>(_a[1]))); break;
        case 8: _t->StopProcessing(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (dataProcessingThread::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dataProcessingThread::HeadingDataReceived)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject dataProcessingThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_dataProcessingThread.data,
      qt_meta_data_dataProcessingThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *dataProcessingThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dataProcessingThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_dataProcessingThread.stringdata0))
        return static_cast<void*>(const_cast< dataProcessingThread*>(this));
    return QThread::qt_metacast(_clname);
}

int dataProcessingThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void dataProcessingThread::HeadingDataReceived(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
