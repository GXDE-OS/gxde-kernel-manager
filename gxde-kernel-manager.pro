QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutwindow.cpp \
    kernelinformation.cpp \
    kernelinstaller.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    aboutwindow.h \
    kernelinformation.h \
    kernelinstaller.h \
    mainwindow.h

FORMS += \
    aboutwindow.ui \
    kernelinstaller.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
unix:!macx: LIBS += -lqtermwidget5

aptsource.path=/etc/apt/sources.list.d/
aptsource.files=$$PWD/AptSources/gxde-kernel-manager.list

aptsourcegpg.path=/etc/apt/trusted.gpg.d/
aptsourcegpg.files=$$PWD/AptSources/gxde-kernel-manager.gpg

debiandesktop.path=/usr/share/applications/
debiandesktop.files=$$PWD/gxde-kernel-manager.desktop

uosdesktop.path=/opt/apps/gxde-kernel-manager/entries/applications
uosdesktop.files=$$PWD/gxde-kernel-manager.desktop

icon.path=/opt/apps/gxde-kernel-manager/
icon.files=$$PWD/icon/icon.svg

target.path=/opt/apps/gxde-kernel-manager/

INSTALLS+= aptsourcegpg aptsource target debiandesktop uosdesktop icon


RESOURCES += \
    Resource.qrc
