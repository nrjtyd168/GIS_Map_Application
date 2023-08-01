QT       += core gui opengl sql
QT       += network widgets
requires(qtConfig(udpsocket))

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -lArcGISQt5Hack

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 15) {
        error("$$TARGET requires Qt 5.15.2")
    }
        equals(QT_MINOR_VERSION, 15) : lessThan(QT_PATCH_VERSION, 2) {
                error("$$TARGET requires Qt 5.15.2")
        }
}

ARCGIS_RUNTIME_VERSION = 100.13.1
include($$PWD/arcgisruntime.pri)
include(/home/niraj/arcgis/runtime_sdk/qt100.13.1/sdk/ideintegration/esri_runtime_qt.pri)

win32:CONFIG += \
    embed_manifest_exe

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    annotations.cpp \
    distancebearing.cpp \
    main.cpp \
    mainwindow.cpp \
    waypoints.cpp

HEADERS += \
    annotations.h \
    distancebearing.h \
    mainwindow.h \
    waypoints.h

FORMS += \
    annotations.ui \
    distancebearing.ui \
    mainwindow.ui \
    waypoints.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Resources.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/multicastreceiver
INSTALLS += target
