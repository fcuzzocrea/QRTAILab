/****************************************************************************
** Meta object code from reading C++ file 'meters_manager.h'
**
** Created: Sun Jan 6 12:44:26 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/meters_manager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'meters_manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_GetMeterDataThread[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      49,   20,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GetMeterDataThread[] = {
    "GetMeterDataThread\0\0arg,targetthread,meterwindow\0"
    "start(void*,TargetThread*,QRL_MeterWindow*)\0"
};

const QMetaObject GetMeterDataThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_GetMeterDataThread,
      qt_meta_data_GetMeterDataThread, 0 }
};

const QMetaObject *GetMeterDataThread::metaObject() const
{
    return &staticMetaObject;
}

void *GetMeterDataThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GetMeterDataThread))
	return static_cast<void*>(const_cast< GetMeterDataThread*>(this));
    return QThread::qt_metacast(_clname);
}

int GetMeterDataThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: start((*reinterpret_cast< void*(*)>(_a[1])),(*reinterpret_cast< TargetThread*(*)>(_a[2])),(*reinterpret_cast< QRL_MeterWindow*(*)>(_a[3]))); break;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_QRL_MetersManager[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      39,   34,   18,   18, 0x0a,
      74,   18,   18,   18, 0x0a,
     100,   18,   18,   18, 0x0a,
     118,   18,   18,   18, 0x0a,
     136,   18,   18,   18, 0x0a,
     153,   18,   18,   18, 0x0a,
     174,   18,   18,   18, 0x0a,
     195,   18,   18,   18, 0x0a,
     218,   18,   18,   18, 0x0a,
     244,   18,   18,   18, 0x0a,
     270,   18,   18,   18, 0x0a,
     293,   18,   18,   18, 0x0a,
     320,   18,   18,   18, 0x0a,
     345,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QRL_MetersManager[] = {
    "QRL_MetersManager\0\0showMeter(int)\0"
    "item\0showMeterOptions(QListWidgetItem*)\0"
    "changeRefreshRate(double)\0changeMin(double)\0"
    "changeMax(double)\0changeMeter(int)\0"
    "changeThermoColor1()\0changeThermoColor2()\0"
    "changePipeWith(double)\0changeAlarmThermoColor1()\0"
    "changeAlarmThermoColor2()\0"
    "enableThermoAlarm(int)\0"
    "changeThermoColorType(int)\0"
    "changeAlarmLevel(double)\0"
    "changeThermoDirection(int)\0"
};

const QMetaObject QRL_MetersManager::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QRL_MetersManager,
      qt_meta_data_QRL_MetersManager, 0 }
};

const QMetaObject *QRL_MetersManager::metaObject() const
{
    return &staticMetaObject;
}

void *QRL_MetersManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QRL_MetersManager))
	return static_cast<void*>(const_cast< QRL_MetersManager*>(this));
    return QDialog::qt_metacast(_clname);
}

int QRL_MetersManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showMeter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: showMeterOptions((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 2: changeRefreshRate((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: changeMin((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: changeMax((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: changeMeter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: changeThermoColor1(); break;
        case 7: changeThermoColor2(); break;
        case 8: changePipeWith((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: changeAlarmThermoColor1(); break;
        case 10: changeAlarmThermoColor2(); break;
        case 11: enableThermoAlarm((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: changeThermoColorType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: changeAlarmLevel((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 14: changeThermoDirection((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 15;
    }
    return _id;
}
