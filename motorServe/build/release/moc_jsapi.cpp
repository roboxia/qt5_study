/****************************************************************************
** Meta object code from reading C++ file 'jsapi.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../jsapi.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jsapi.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_jsapi_t {
    QByteArrayData data[16];
    char stringdata0[151];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_jsapi_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_jsapi_t qt_meta_stringdata_jsapi = {
    {
QT_MOC_LITERAL(0, 0, 5), // "jsapi"
QT_MOC_LITERAL(1, 6, 10), // "jsWriteReg"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 4), // "addr"
QT_MOC_LITERAL(4, 23, 5), // "value"
QT_MOC_LITERAL(5, 29, 9), // "jsReadReg"
QT_MOC_LITERAL(6, 39, 13), // "exitServerApp"
QT_MOC_LITERAL(7, 53, 12), // "jlinkConnect"
QT_MOC_LITERAL(8, 66, 6), // "config"
QT_MOC_LITERAL(9, 73, 10), // "closeJlink"
QT_MOC_LITERAL(10, 84, 7), // "openRTT"
QT_MOC_LITERAL(11, 92, 8), // "closeRTT"
QT_MOC_LITERAL(12, 101, 11), // "openRTTPlot"
QT_MOC_LITERAL(13, 113, 12), // "closeRTTPlot"
QT_MOC_LITERAL(14, 126, 11), // "openRegPlot"
QT_MOC_LITERAL(15, 138, 12) // "closeRegPlot"

    },
    "jsapi\0jsWriteReg\0\0addr\0value\0jsReadReg\0"
    "exitServerApp\0jlinkConnect\0config\0"
    "closeJlink\0openRTT\0closeRTT\0openRTTPlot\0"
    "closeRTTPlot\0openRegPlot\0closeRegPlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_jsapi[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x0a /* Public */,
       5,    1,   74,    2, 0x0a /* Public */,
       6,    0,   77,    2, 0x0a /* Public */,
       7,    1,   78,    2, 0x0a /* Public */,
       9,    0,   81,    2, 0x0a /* Public */,
      10,    0,   82,    2, 0x0a /* Public */,
      11,    0,   83,    2, 0x0a /* Public */,
      12,    0,   84,    2, 0x0a /* Public */,
      13,    0,   85,    2, 0x0a /* Public */,
      14,    0,   86,    2, 0x0a /* Public */,
      15,    0,   87,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::QString, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::QByteArray, QMetaType::QByteArray,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void jsapi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        jsapi *_t = static_cast<jsapi *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { QString _r = _t->jsWriteReg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 1: { QString _r = _t->jsReadReg((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->exitServerApp(); break;
        case 3: { QByteArray _r = _t->jlinkConnect((*reinterpret_cast< QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->closeJlink(); break;
        case 5: _t->openRTT(); break;
        case 6: _t->closeRTT(); break;
        case 7: _t->openRTTPlot(); break;
        case 8: _t->closeRTTPlot(); break;
        case 9: _t->openRegPlot(); break;
        case 10: _t->closeRegPlot(); break;
        default: ;
        }
    }
}

const QMetaObject jsapi::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_jsapi.data,
      qt_meta_data_jsapi,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *jsapi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *jsapi::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_jsapi.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int jsapi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
