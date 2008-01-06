/****************************************************************************
** Meta object code from reading C++ file 'meter_window.h'
**
** Created: Sun Jan 6 12:44:24 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/meter_window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'meter_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QRL_MeterWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       1,   15, // enums/sets

 // slots: signature, parameters, type, tag, flags
      23,   17,   16,   16, 0x09,

 // enums: name, flags, count, data
      48, 0x0,    3,   19,

 // enum data: key, value
      59, uint(QRL_MeterWindow::THERMO),
      66, uint(QRL_MeterWindow::DIAL),
      71, uint(QRL_MeterWindow::LCD),

       0        // eod
};

static const char qt_meta_stringdata_QRL_MeterWindow[] = {
    "QRL_MeterWindow\0\0event\0closeEvent(QCloseEvent*)\0"
    "Meter_Type\0THERMO\0DIAL\0LCD\0"
};

const QMetaObject QRL_MeterWindow::staticMetaObject = {
    { &QMdiSubWindow::staticMetaObject, qt_meta_stringdata_QRL_MeterWindow,
      qt_meta_data_QRL_MeterWindow, 0 }
};

const QMetaObject *QRL_MeterWindow::metaObject() const
{
    return &staticMetaObject;
}

void *QRL_MeterWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QRL_MeterWindow))
	return static_cast<void*>(const_cast< QRL_MeterWindow*>(this));
    return QMdiSubWindow::qt_metacast(_clname);
}

int QRL_MeterWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMdiSubWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
