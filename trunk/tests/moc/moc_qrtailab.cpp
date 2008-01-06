/****************************************************************************
** Meta object code from reading C++ file 'qrtailab.h'
**
** Created: Sun Jan 6 12:44:21 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/qrtailab.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qrtailab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_TargetThread[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       2,   25, // enums/sets

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   13,   13,   13, 0x0a,
      48,   13,   13,   13, 0x0a,

 // enums: name, flags, count, data
      62, 0x0,    7,   33,
      75, 0x0,   10,   47,

 // enum data: key, value
      84, uint(TargetThread::PARAMS_MANAGER),
      99, uint(TargetThread::SCOPES_MANAGER),
     114, uint(TargetThread::LOGS_MANAGER),
     127, uint(TargetThread::ALOGS_MANAGER),
     141, uint(TargetThread::LEDS_MANAGER),
     154, uint(TargetThread::METERS_MANAGER),
     169, uint(TargetThread::SYNCHS_MANAGER),
     184, uint(TargetThread::CONNECT_TO_TARGET),
     202, uint(TargetThread::CONNECT_TO_TARGET_WITH_PROFILE),
     233, uint(TargetThread::DISCONNECT_FROM_TARGET),
     256, uint(TargetThread::START_TARGET),
     269, uint(TargetThread::STOP_TARGET),
     281, uint(TargetThread::UPDATE_PARAM),
     294, uint(TargetThread::GET_TARGET_TIME),
     310, uint(TargetThread::BATCH_DOWNLOAD),
     325, uint(TargetThread::GET_PARAMS),
     336, uint(TargetThread::CLOSE),

       0        // eod
};

static const char qt_meta_stringdata_TargetThread[] = {
    "TargetThread\0\0statusBarMessage(QString)\0"
    "start()\0getOrder(int)\0Manager_Type\0"
    "Commands\0PARAMS_MANAGER\0SCOPES_MANAGER\0"
    "LOGS_MANAGER\0ALOGS_MANAGER\0LEDS_MANAGER\0"
    "METERS_MANAGER\0SYNCHS_MANAGER\0"
    "CONNECT_TO_TARGET\0CONNECT_TO_TARGET_WITH_PROFILE\0"
    "DISCONNECT_FROM_TARGET\0START_TARGET\0"
    "STOP_TARGET\0UPDATE_PARAM\0GET_TARGET_TIME\0"
    "BATCH_DOWNLOAD\0GET_PARAMS\0CLOSE\0"
};

const QMetaObject TargetThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_TargetThread,
      qt_meta_data_TargetThread, 0 }
};

const QMetaObject *TargetThread::metaObject() const
{
    return &staticMetaObject;
}

void *TargetThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TargetThread))
	return static_cast<void*>(const_cast< TargetThread*>(this));
    return QThread::qt_metacast(_clname);
}

int TargetThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: statusBarMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: start(); break;
        case 2: getOrder((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TargetThread::statusBarMessage(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
