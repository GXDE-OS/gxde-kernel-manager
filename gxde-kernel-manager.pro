QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutwindow.cpp \
    aptpkginfo.cpp \
    kernelinformation.cpp \
    kernelinformationdialog.cpp \
    kernelinstaller.cpp \
    main.cpp \
    mainwindow.cpp \
    programinfo.cpp

HEADERS += \
    aboutwindow.h \
    aptpkginfo.h \
    kernelinformation.h \
    kernelinformationdialog.h \
    kernelinstaller.h \
    mainwindow.h \
    programinfo.h

FORMS += \
    aboutwindow.ui \
    kernelinformationdialog.ui \
    kernelinstaller.ui \
    mainwindow.ui

# 更新翻译
system(lrelease translation/gxde-kernel-manager_zh_CN.ts translation/gxde-kernel-manager_zh_CN.qm)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
unix:!macx: LIBS += -lqtermwidget5

# 使用 GXDE 系统源里含有的内核
#aptsource.path = /etc/apt/sources.list.d/
#aptsource.files = $$PWD/AptSources/gxde-kernel-manager.list

aptsourcegpg.path = /etc/apt/trusted.gpg.d/
aptsourcegpg.files = $$PWD/AptSources/gxde-kernel-manager.gpg

debiandesktop.path = /usr/share/applications/
debiandesktop.files = $$PWD/gxde-kernel-manager.desktop

#uosdesktop.path = /opt/apps/gxde-kernel-manager/entries/applications
#uosdesktop.files = $$PWD/gxde-kernel-manager.desktop

icon.path = /usr/share/icons/
icon.files = $$PWD/icon/gxde-kernel-manager.svg
#icon.path = /opt/apps/gxde-kernel-manager/
#icon.files = $$PWD/icon/icon.svg

# GXDE 下不需要该脚本
#targetrunner.path = /opt/apps/gxde-kernel-manager/
#targetrunner.files = $$PWD/gxde-kernel-manager-runner.sh

#libinclude.path = /opt/apps/gxde-kernel-manager/
#libinclude.files = $$PWD/lib

#target.path = /opt/apps/gxde-kernel-manager/
target.path = /usr/bin

sysbro.path = /usr/share/GXDE/gxde-system-assistant/tool-extensions
sysbro.files = $$PWD/gxde-kernel-manager.desktop

#INSTALLS += aptsourcegpg aptsource target debiandesktop uosdesktop icon targetrunner
INSTALLS += target icon sysbro

TRANSLATIONS += translation/gxde-kernel-manager_zh_CN.ts

RESOURCES += \
    Resource.qrc
