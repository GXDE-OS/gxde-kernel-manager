#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include "programinfo.h"

#include <QDateTime>
#include <QProcessEnvironment>
#include <QFile>

AboutWindow::AboutWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);


    QString info = "";
    info += "<pre>";
    info += tr("A kernel manager allows users to install or remove kernels easily.") + "\n\n";
    info += "<b>" + tr("Warning: You may damage your system unless you know what you will do!") + "</b>\n";
    info += tr("Built Time: ") + QString(__DATE__) + " " + __TIME__ + "\n";
    info += tr("QQ Group: 881201853") + "\n";
    info += "</pre>";
    /*info += "<hr/>";
    info += ProgramInfo::updateInfo();
    info += "<hr/>";*/
    info += "<h3>" + tr("Website") + "</h3>";
    info += "<p>Gitee: <a href='https://gitee.com/GXDE-OS/gxde-kernel-manager'>https://gitee.com/GXDE-OS/gxde-kernel-manager</a></p>";
    info += "<p>Github: <a href='https://github.com/GXDE-OS/gxde-kernel-manager'>https://github.com/GXDE-OS/gxde-kernel-manager</a></p>";
    info += "<p>GXDE Homepage: <a href='https://www.gxde.top'>https://www.gxde.top</a></p>";
    info += "<hr/>\n";
    info += "<h1>©2023～" + QDateTime::currentDateTime().toString("yyyy") + " gfdgd xi</h1>\n";
    ui->textBrowser->setHtml(info);
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::on_m_closeButton_clicked()
{
    this->close();
}


void AboutWindow::on_m_logoShower_linkActivated(const QString &link)
{
    ui->m_logoShower->setText("<html><head/><body><p><a href='https://www.gxde.org'><img width=128 src=\":/icon/icon1.png\"/></a></p></body></html>");
}

