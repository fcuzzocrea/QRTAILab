/****************************************************************************
** Meta object code from reading C++ file 'led_window.h'
**
** Created: Sun Jan 6 12:44:29 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/led_window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'led_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QLed[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       2,   25, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x0a,
      21,    5,    5,    5, 0x0a,
      38,    5,    5,    5, 0x0a,

 // properties: name, type, flags
      57,   52, 0x01095103,
      70,   63, 0x43095103,

       0        // eod
};

static const char qt_meta_stringdata_QLed[] = {
    "QLed\0\0setValue(bool)\0setColor(QColor)\0"
    "toggleValue()\0bool\0value\0QColor\0color\0"
};

const QMetaObject QLed::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QLed,
      qt_meta_data_QLed, 0 }
};

const QMetaObject *QLed::metaObject() const
{
    return &staticMetaObject;
}

void *QLed::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QLed))
	return static_cast<void*>(const_cast< QLed*>(this));
    return QWidget::qt_metacast(_clname);
}

int QLed::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setValue((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: setColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: toggleValue(); break;
        }
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = value(); break;
        case 1: *reinterpret_cast< QColor*>(_v) = color(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setValue(*reinterpret_cast< bool*>(_v)); break;
        case 1: setColor(*reinterpret_cast< QColor*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
static const uint qt_meta_data_QRL_LedWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_QRL_LedWindow[] = {
    "QRL_LedWindow\0\0event\0closeEvent(QCloseEvent*)\0"
};

const QMetaObject QRL_LedWindow::staticMetaObject = {
    { &QMdiSubWindow::staticMetaObject, qt_meta_stringdata_QRL_LedWindow,
      qt_meta_data_QRL_LedWindow, 0 }
};

const QMetaObject *QRL_LedWindow::metaObject() const
{
    return &staticMetaObject;
}

void *QRL_LedWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QRL_LedWindow))
	return static_cast<void*>(const_cast< QRL_LedWindow*>(this));
    return QMdiSubWindow::qt_metacast(_clname);
}

int QRL_LedWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
