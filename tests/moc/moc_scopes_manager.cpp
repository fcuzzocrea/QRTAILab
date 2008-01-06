/****************************************************************************
** Meta object code from reading C++ file 'scopes_manager.h'
**
** Created: Sun Jan 6 12:44:37 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/scopes_manager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scopes_manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_GetScopeDataThread[] = {

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

static const char qt_meta_stringdata_GetScopeDataThread[] = {
    "GetScopeDataThread\0\0arg,targetthread,scopewindow\0"
    "start(void*,TargetThread*,QRL_ScopeWindow*)\0"
};

const QMetaObject GetScopeDataThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_GetScopeDataThread,
      qt_meta_data_GetScopeDataThread, 0 }
};

const QMetaObject *GetScopeDataThread::metaObject() const
{
    return &staticMetaObject;
}

void *GetScopeDataThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GetScopeDataThread))
	return static_cast<void*>(const_cast< GetScopeDataThread*>(this));
    return QThread::qt_metacast(_clname);
}

int GetScopeDataThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: start((*reinterpret_cast< void*(*)>(_a[1])),(*reinterpret_cast< TargetThread*(*)>(_a[2])),(*reinterpret_cast< QRL_ScopeWindow*(*)>(_a[3]))); break;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_QRL_ScopesManager[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      39,   34,   18,   18, 0x0a,
      74,   18,   18,   18, 0x0a,
     100,   18,   18,   18, 0x0a,
     125,   18,   18,   18, 0x0a,
     148,   18,   18,   18, 0x0a,
     166,   18,   18,   18, 0x0a,
     180,   18,   18,   18, 0x0a,
     196,   18,   18,   18, 0x0a,
     212,   18,   18,   18, 0x0a,
     233,   18,   18,   18, 0x0a,
     252,   18,   18,   18, 0x0a,
     277,   18,   18,   18, 0x0a,
     298,   18,   18,   18, 0x0a,
     315,   18,   18,   18, 0x0a,
     339,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QRL_ScopesManager[] = {
    "QRL_ScopesManager\0\0showScope(int)\0"
    "item\0showScopeOptions(QListWidgetItem*)\0"
    "changeRefreshRate(double)\0"
    "changeDataPoints(double)\0"
    "changeSaveTime(double)\0changeDX(QString)\0"
    "startSaving()\0stopSaving(int)\0"
    "setOptions(int)\0setCurrentTrace(int)\0"
    "changeTraceColor()\0changeTraceWidth(double)\0"
    "changeOffset(double)\0changeDy(double)\0"
    "changeDisplayModus(int)\0changeDirection(int)\0"
};

const QMetaObject QRL_ScopesManager::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QRL_ScopesManager,
      qt_meta_data_QRL_ScopesManager, 0 }
};

const QMetaObject *QRL_ScopesManager::metaObject() const
{
    return &staticMetaObject;
}

void *QRL_ScopesManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QRL_ScopesManager))
	return static_cast<void*>(const_cast< QRL_ScopesManager*>(this));
    return QDialog::qt_metacast(_clname);
}

int QRL_ScopesManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showScope((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: showScopeOptions((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 2: changeRefreshRate((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: changeDataPoints((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: changeSaveTime((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: changeDX((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: startSaving(); break;
        case 7: stopSaving((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: setOptions((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: setCurrentTrace((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: changeTraceColor(); break;
        case 11: changeTraceWidth((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: changeOffset((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: changeDy((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 14: changeDisplayModus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: changeDirection((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 16;
    }
    return _id;
}
