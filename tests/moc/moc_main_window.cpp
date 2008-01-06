/****************************************************************************
** Meta object code from reading C++ file 'main_window.h'
**
** Created: Sun Jan 6 12:44:40 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/main_window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QRL_connectDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      28,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QRL_connectDialog[] = {
    "QRL_connectDialog\0\0accept()\0reject()\0"
};

const QMetaObject QRL_connectDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QRL_connectDialog,
      qt_meta_data_QRL_connectDialog, 0 }
};

const QMetaObject *QRL_connectDialog::metaObject() const
{
    return &staticMetaObject;
}

void *QRL_connectDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QRL_connectDialog))
	return static_cast<void*>(const_cast< QRL_connectDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QRL_connectDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: accept(); break;
        case 1: reject(); break;
        }
        _id -= 2;
    }
    return _id;
}
static const uint qt_meta_data_QRL_MainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x09,
      24,   15,   15,   15, 0x09,
      43,   15,   15,   15, 0x09,
      59,   15,   15,   15, 0x09,
      78,   15,   15,   15, 0x09,
      92,   15,   15,   15, 0x09,
     100,   15,   15,   15, 0x09,
     107,   15,   15,   15, 0x09,
     127,   15,   15,   15, 0x09,
     147,   15,   15,   15, 0x09,
     165,   15,   15,   15, 0x09,
     189,   15,   15,   15, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_QRL_MainWindow[] = {
    "QRL_MainWindow\0\0about()\0disconnectDialog()\0"
    "connectDialog()\0connect_WProfile()\0"
    "saveProfile()\0start()\0stop()\0"
    "showMetersManager()\0showScopesManager()\0"
    "showLedsManager()\0showParametersManager()\0"
    "setStatusBarMessage(QString)\0"
};

const QMetaObject QRL_MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QRL_MainWindow,
      qt_meta_data_QRL_MainWindow, 0 }
};

const QMetaObject *QRL_MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *QRL_MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QRL_MainWindow))
	return static_cast<void*>(const_cast< QRL_MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QRL_MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: about(); break;
        case 1: disconnectDialog(); break;
        case 2: connectDialog(); break;
        case 3: connect_WProfile(); break;
        case 4: saveProfile(); break;
        case 5: start(); break;
        case 6: stop(); break;
        case 7: showMetersManager(); break;
        case 8: showScopesManager(); break;
        case 9: showLedsManager(); break;
        case 10: showParametersManager(); break;
        case 11: setStatusBarMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        }
        _id -= 12;
    }
    return _id;
}
