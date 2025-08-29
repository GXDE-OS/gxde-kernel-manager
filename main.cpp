#include "mainwindow.h"

#include "programinfo.h"

#include <QApplication>

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    if(ProgramInfo::systemLANG() == ProgramInfo::LANG::zh_CN) {
        translator.load(":/translations/gxde-kernel-manager_zh_CN.qm");
    }
    a.installTranslator(&translator);
#ifdef __linux__
    // 风险提示
    if(!QFile::exists(QDir::homePath() + "/.config/gxde-kernel-manager/read_already")) {
        QMessageBox::warning(NULL, QObject::tr("Waring"), QObject::tr("It is danger because it may make your system unboot.\nPress 'OK' to countinue"));
    }
    MainWindow w;
    w.show();
    return a.exec();
#else
    // 其它系统不支持该工具
    QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("It unsupport Windows"));
    return a.exec();
#endif


}
