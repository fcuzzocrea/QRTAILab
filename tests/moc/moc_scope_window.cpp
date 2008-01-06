/****************************************************************************
** Meta object code from reading C++ file 'scope_window.h'
**
** Created: Sun Jan 6 12:44:35 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/scope_window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scope_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QRL_ScopeWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      33,   16,   16,   16, 0x0a,
      49,   43,   16,   16, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_QRL_ScopeWindow[] = {
    "QRL_ScopeWindow\0\0stopSaving(int)\0"
    "refresh()\0event\0closeEvent(QCloseEvent*)\0"
};

const QMetaObject QRL_ScopeWindow::staticMetaObject = {
    { &QMdiSubWindow::staticMetaObject, qt_meta_stringdata_QRL_ScopeWindow,
      qt_meta_data_QRL_ScopeWindow, 0 }
};

const QMetaObject *QRL_ScopeWindow::metaObject() const
{
    return &staticMetaObject;
}

void *QRL_ScopeWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QRL_ScopeWindow))
	return static_cast<void*>(const_cast< QRL_ScopeWindow*>(this));
    return QMdiSubWindow::qt_metacast(_clname);
}

int QRL_ScopeWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMdiSubWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: stopSaving((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: refresh(); break;
        case 2: closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QRL_ScopeWindow::stopSaving(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
