#include "kernelinstaller.h"
#include "ui_kernelinstaller.h"

#include <ctime>
#include <cstdlib>
#include <QFile>

#define MAX_TMP_NUM 1024
#define MIN_TMP_NUM 0

KernelInstaller::KernelInstaller(QStringList kernelList, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KernelInstaller)
{
    ui->setupUi(this);
    this->kernelList = kernelList;
    terminal = new QTermWidget(0);
    terminal->setColorScheme("DarkPastels");
    terminal->setAutoClose(true);
    ui->verticalLayout->addWidget(terminal);
    StartInstaller();
}

void KernelInstaller::StartInstaller()
{
    terminal->setShellProgram("/usr/bin/pkexec");
    // 随机生成目录
    srand(time(0));
    int randomNumber = (rand() % (MAX_TMP_NUM - MIN_TMP_NUM)) + 1;  // 左闭右闭
    QString tempPath = tempDir + "/gxde-kernel-manager-installer-" + QString::number(randomNumber) + ".sh";
    QFile file(tempPath);
    file.open(QFile::WriteOnly);
    file.write(BuildKernelInstallerBash(this->kernelList, tempPath).toUtf8());
    file.close();
    terminal->setArgs(QStringList() << "bash" << tempPath);
    terminal->startShellProgram();
}

QString KernelInstaller::BuildKernelInstallerBash(QStringList kernelList, QString tempFilePath) const
{
    // 转换数据
    QString kernel = "";
    foreach (QString name, kernelList) {
        kernel += name + " ";
    }
    QFile file(":/shell/kernel-installer-template.sh");
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    data = data.replace("${KernelList}", kernel);
    data += "\nrm -v '" + tempFilePath + "'";
    file.close();
    return data;
}

KernelInstaller::~KernelInstaller()
{
    delete ui;
}
