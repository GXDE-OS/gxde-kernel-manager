#include "mainwindow.h"

#include <QApplication>

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 风险提示
    if(!QFile::exists(QDir::homePath() + "/.config/gxde-kernel-manager/read_already")) {
        QMessageBox::warning(NULL, QObject::tr("Waring"), QObject::tr("It is danger because it may make your system unboot.\nPress 'OK' to countinue"));
    }
    MainWindow w;
    w.show();
    return a.exec();
}
