/****************************************************************************
** Meta object code from reading C++ file 'qsshsocket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../Desktop/tcc_gui/tcc_gui/qsshsocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qsshsocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QSshSocket_t {
    QByteArrayData data[17];
    char stringdata0[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QSshSocket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QSshSocket_t qt_meta_stringdata_QSshSocket = {
    {
QT_MOC_LITERAL(0, 0, 10), // "QSshSocket"
QT_MOC_LITERAL(1, 11, 9), // "connected"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 12), // "disconnected"
QT_MOC_LITERAL(4, 35, 5), // "error"
QT_MOC_LITERAL(5, 41, 20), // "QSshSocket::SshError"
QT_MOC_LITERAL(6, 62, 15), // "commandExecuted"
QT_MOC_LITERAL(7, 78, 7), // "command"
QT_MOC_LITERAL(8, 86, 8), // "response"
QT_MOC_LITERAL(9, 95, 15), // "loginSuccessful"
QT_MOC_LITERAL(10, 111, 14), // "pullSuccessful"
QT_MOC_LITERAL(11, 126, 9), // "localFile"
QT_MOC_LITERAL(12, 136, 10), // "remoteFile"
QT_MOC_LITERAL(13, 147, 14), // "pushSuccessful"
QT_MOC_LITERAL(14, 162, 19), // "workingDirectorySet"
QT_MOC_LITERAL(15, 182, 3), // "cwd"
QT_MOC_LITERAL(16, 186, 3) // "run"

    },
    "QSshSocket\0connected\0\0disconnected\0"
    "error\0QSshSocket::SshError\0commandExecuted\0"
    "command\0response\0loginSuccessful\0"
    "pullSuccessful\0localFile\0remoteFile\0"
    "pushSuccessful\0workingDirectorySet\0"
    "cwd\0run"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QSshSocket[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    1,   61,    2, 0x06 /* Public */,
       6,    2,   64,    2, 0x06 /* Public */,
       9,    0,   69,    2, 0x06 /* Public */,
      10,    2,   70,    2, 0x06 /* Public */,
      13,    2,   75,    2, 0x06 /* Public */,
      14,    1,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    0,   83,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   11,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   11,   12,
    QMetaType::Void, QMetaType::QString,   15,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void QSshSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QSshSocket *_t = static_cast<QSshSocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->error((*reinterpret_cast< QSshSocket::SshError(*)>(_a[1]))); break;
        case 3: _t->commandExecuted((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->loginSuccessful(); break;
        case 5: _t->pullSuccessful((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->pushSuccessful((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->workingDirectorySet((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->run(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QSshSocket::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSshSocket::connected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QSshSocket::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSshSocket::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QSshSocket::*_t)(QSshSocket::SshError );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSshSocket::error)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (QSshSocket::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSshSocket::commandExecuted)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (QSshSocket::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSshSocket::loginSuccessful)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (QSshSocket::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSshSocket::pullSuccessful)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (QSshSocket::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSshSocket::pushSuccessful)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (QSshSocket::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSshSocket::workingDirectorySet)) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject QSshSocket::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_QSshSocket.data,
      qt_meta_data_QSshSocket,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QSshSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QSshSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QSshSocket.stringdata0))
        return static_cast<void*>(const_cast< QSshSocket*>(this));
    return QThread::qt_metacast(_clname);
}

int QSshSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void QSshSocket::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void QSshSocket::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void QSshSocket::error(QSshSocket::SshError _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QSshSocket::commandExecuted(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QSshSocket::loginSuccessful()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void QSshSocket::pullSuccessful(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QSshSocket::pushSuccessful(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QSshSocket::workingDirectorySet(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
