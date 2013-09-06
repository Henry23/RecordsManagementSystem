/****************************************************************************
** Meta object code from reading C++ file 'modifyfielddialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../RecordsManagementSystem/dialogs/modifyfielddialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modifyfielddialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ModifyFieldDialog_t {
    QByteArrayData data[9];
    char stringdata[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ModifyFieldDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ModifyFieldDialog_t qt_meta_stringdata_ModifyFieldDialog = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 32),
QT_MOC_LITERAL(2, 51, 0),
QT_MOC_LITERAL(3, 52, 3),
QT_MOC_LITERAL(4, 56, 6),
QT_MOC_LITERAL(5, 63, 10),
QT_MOC_LITERAL(6, 74, 23),
QT_MOC_LITERAL(7, 98, 9),
QT_MOC_LITERAL(8, 108, 11)
    },
    "ModifyFieldDialog\0on_tableWidgetFields_cellChanged\0"
    "\0row\0column\0modifyName\0modifyTypeLengthDecimal\0"
    "modifyKey\0modifyField\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModifyFieldDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x08,
       5,    0,   44,    2, 0x08,
       6,    0,   45,    2, 0x08,
       7,    0,   46,    2, 0x08,
       8,    3,   47,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString,    2,    2,    2,

       0        // eod
};

void ModifyFieldDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ModifyFieldDialog *_t = static_cast<ModifyFieldDialog *>(_o);
        switch (_id) {
        case 0: _t->on_tableWidgetFields_cellChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->modifyName(); break;
        case 2: _t->modifyTypeLengthDecimal(); break;
        case 3: _t->modifyKey(); break;
        case 4: _t->modifyField((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject ModifyFieldDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ModifyFieldDialog.data,
      qt_meta_data_ModifyFieldDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *ModifyFieldDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModifyFieldDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModifyFieldDialog.stringdata))
        return static_cast<void*>(const_cast< ModifyFieldDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int ModifyFieldDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
