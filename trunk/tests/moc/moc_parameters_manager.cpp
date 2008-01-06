/****************************************************************************
** Meta object code from reading C++ file 'parameters_manager.h'
**
** Created: Sun Jan 6 12:44:43 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/parameters_manager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'parameters_manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QRL_ParametersManager[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       1,   35, // enums/sets

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x0a,
      43,   38,   22,   22, 0x0a,
      82,   38,   22,   22, 0x0a,
     124,   22,   22,   22, 0x0a,
     143,   22,   22,   22, 0x0a,

 // enums: name, flags, count, data
     169, 0x0,    5,   39,

 // enum data: key, value
     181, uint(QRL_ParametersManager::rt_SCALAR),
     191, uint(QRL_ParametersManager::rt_VECTOR),
     201, uint(QRL_ParametersManager::rt_MATRIX_ROW_MAJOR),
     221, uint(QRL_ParametersManager::rt_MATRIX_COL_MAJOR),
     241, uint(QRL_ParametersManager::rt_MATRIX_COL_MAJOR_ND),

       0        // eod
};

static const char qt_meta_stringdata_QRL_ParametersManager[] = {
    "QRL_ParametersManager\0\0batchMode(int)\0"
    "item\0showTunableParameter(QListWidgetItem*)\0"
    "changeTunableParameter(QTableWidgetItem*)\0"
    "uploadParameters()\0downloadBatchParameters()\0"
    "Param_Class\0rt_SCALAR\0rt_VECTOR\0"
    "rt_MATRIX_ROW_MAJOR\0rt_MATRIX_COL_MAJOR\0"
    "rt_MATRIX_COL_MAJOR_ND\0"
};

const QMetaObject QRL_ParametersManager::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QRL_ParametersManager,
      qt_meta_data_QRL_ParametersManager, 0 }
};

const QMetaObject *QRL_ParametersManager::metaObject() const
{
    return &staticMetaObject;
}

void *QRL_ParametersManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QRL_ParametersManager))
	return static_cast<void*>(const_cast< QRL_ParametersManager*>(this));
    return QDialog::qt_metacast(_clname);
}

int QRL_ParametersManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: batchMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: showTunableParameter((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 2: changeTunableParameter((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 3: uploadParameters(); break;
        case 4: downloadBatchParameters(); break;
        }
        _id -= 5;
    }
    return _id;
}
