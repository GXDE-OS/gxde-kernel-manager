#include "kernelinstaller.h"
#include "ui_kernelinstaller.h"

#include <ctime>
#include <cstdlib>
#include <QFile>
#include <unistd.h>
#include <QDebug>

#define MAX_TMP_NUM 1024
#define MIN_TMP_NUM 0

KernelInstaller::KernelInstaller(Option option, QStringList kernelList, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KernelInstaller)
{
    ui->setupUi(this);
    runOption = option;
    // 修改提示文本
    QString kernel = "";
    foreach (QString name, kernelList) {
        kernel += name + " ";
    }

    switch(runOption) {
        case Option::Install:
            ui->m_status->setText(tr("Try to install ") + kernel);
            break;
        case Option::Remove:
            ui->m_status->setText(tr("Try to remove ") + kernel);
            break;
    }


    this->kernelList = kernelList;
    terminal = new QTermWidget(0);
    terminal->setColorScheme("DarkPastels");
    terminal->setAutoClose(false);
    ui->verticalLayout->addWidget(terminal);
    StartInstaller();
}

void KernelInstaller::StartInstaller()
{
    // 随机生成目录
    srand(time(0));
    int randomNumber = (rand() % (MAX_TMP_NUM - MIN_TMP_NUM)) + 1;  // 左闭右闭
    QString tempPath = tempDir + "/gxde-kernel-manager-installer-" + QString::number(randomNumber) + ".sh";
    QString tempLoaderPath = tempDir + "/gxde-kernel-manager-root-loader-installer-" + QString::number(randomNumber) + ".sh";
    QFile file(tempPath);
    file.open(QFile::WriteOnly);
    file.write(BuildKernelInstallerBash(this->kernelList, tempPath).toUtf8());
    file.close();
    QFile loaderFile(tempLoaderPath);
    loaderFile.open(QFile::WriteOnly);
    loaderFile.write(BuildKernelRootLoaderInstallerBash(tempPath, tempLoaderPath).toUtf8());
    loaderFile.close();
    terminal->setShellProgram("/usr/bin/bash");
    terminal->setArgs(QStringList() << tempLoaderPath);
    terminal->startShellProgram();
    processID = terminal->getShellPID();
    // 使用 QTimer 用于判断内核是否安装完成
    QTimer *runStatusTimer = new QTimer();
    runStatusTimer->setInterval(100);
    connect(runStatusTimer, &QTimer::timeout, this, &KernelInstaller::CheckInstallerStatusTimer);
    runStatusTimer->start();
}

QString KernelInstaller::BuildKernelInstallerBash(QStringList kernelList, QString tempFilePath) const
{
    // 转换数据
    QString kernel = "";
    foreach (QString name, kernelList) {
        kernel += name + " ";
    }
    QString filePath = ":/shell/kernel-installer-template.sh";
    switch(runOption) {
        case Option::Install:
            filePath = ":/shell/kernel-installer-template.sh";
            break;
        case Option::Remove:
            filePath = ":/shell/kernel-installer-remove-template.sh";
            break;
    }

    QFile file(filePath);
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    data = data.replace("${KernelList}", kernel);
    data = data.replace("${kernelInstallerShellTempPath}", tempFilePath);
    file.close();
    return data;
}

QString KernelInstaller::BuildKernelRootLoaderInstallerBash(QString tempFilePath, QString tempLoaderFilePath) const
{
    QFile file(":/shell/kernel-installer-root-loader-template.sh");
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    data = data.replace("${kernelInstallerShellTempPath}", tempFilePath);
    data = data.replace("${kernelInstallerRootLoaderShellTempPath}", tempLoaderFilePath);
    file.close();
    return data;
}

/*
 * -1 为进程正在运行（正常）
 * -2 为无法读取进程信息（异常）
*/
int KernelInstaller::CheckInstallerStatus()
{
    if(QFile::exists("/proc/" + QString::number(processID))) {
        // 如果进程存在
        return -1;
    }
    QFile file("/tmp/gxde-kernel-manager-installer-status");
    if(!file.exists()) {
        return -2;
    }
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();
    data = data.replace("\n", "").replace(" ", "");
    return data.toInt();
}

void KernelInstaller::CheckInstallerStatusTimer()
{
    int status = CheckInstallerStatus();
    if(status == -1) {
        return;
    }
    // 安装完成
    if(status == 0) {
        ui->m_status->setText(tr("Done"));
        ui->m_icon->setText("<img src=':/icon/dialog-ok.svg'>");
        return;
    }
    ui->m_icon->setText("<img src=':/icon/dialog-error.svg'>");
    ui->m_status->setText(tr("Something error, exit code: ") + QString::number(status));
}

KernelInstaller::~KernelInstaller()
{
    delete ui;
}
