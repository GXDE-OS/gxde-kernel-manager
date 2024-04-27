#include "aboutwindow.h"
#include "ui_aboutwindow.h"

#include <QDateTime>

AboutWindow::AboutWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    QString info = "";
    info += "<pre>";
    info += tr("A kernel manager allows users to install or remove kernels easily.") + "\n\n";
    info += "<b>" + tr("Warning: You may damage your system unless you know what you will do!") + "</b>\n";
    info += "</pre>";
    info += "<hr/>";
    info += "<h3>" + tr("Website") + "</h3>";
    info += "<p>Gitee: <a href='https://gitee.com/GXDE-OS/gxde-kernel-manager'>https://gitee.com/GXDE-OS/gxde-kernel-manager</a></p>";
    info += "<p>Github: <a href='https://github.com/GXDE-OS/gxde-kernel-manager'>https://github.com/GXDE-OS/gxde-kernel-manager</a></p>";
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

