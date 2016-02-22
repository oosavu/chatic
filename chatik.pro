TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    chatmanager.cpp \
    user.cpp \
    tinyaes.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    ContactList.qml\
    main.qml \
    TabComponent.qml

HEADERS += \
    chatmanager.h \
    user.h \
    tinyaes.h
