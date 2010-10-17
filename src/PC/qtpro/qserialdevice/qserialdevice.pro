#
# QSerialDevice library project file
#
#   Edit by Tankry, tankery.chen@gmail.com

TEMPLATE = lib
CONFIG += staticlib
#CONFIG += dll
QT -= gui

OBJECTS_DIR = obj
MOC_DIR = moc
DESTDIR = lib
CONFIG(debug, debug|release) {
    TARGET = qserialdeviced
} else {
    TARGET = qserialdevice
}

#TODO: here in future replace
contains( CONFIG, dll ) {
    win32:DEFINES += QSERIALDEVICE_SHARED
}

include(../../qserialdevice-qserialdevice/src/qserialdevice/qserialdevice.pri)
include(../../qserialdevice-qserialdevice/src/qserialdeviceenumerator/qserialdeviceenumerator.pri)

win32 {
    LIBS += -lsetupapi -luuid -ladvapi32
}
unix:!macx {
    LIBS += -ludev
}
