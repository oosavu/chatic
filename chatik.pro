TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    chatmanager.cpp \
    tinyaes.cpp \
    userlistmodel.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    main.qml

HEADERS += \
    chatmanager.h \
    tinyaes.h \
    userlistmodel.h
