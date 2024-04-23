#include "kernelinstaller.h"
#include "ui_kernelinstaller.h"

#include <ctime>
#include <cstdlib>
#include <QFile>

#define MAX_TMP_NUM 1024
#define MIN_TMP_NUM 1024

KernelInstaller::KernelInstaller(QStringList kernelList, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KernelInstaller)
{
    ui->setupUi(this);
    this->kernelList = kernelList;
    terminal = new QTermWidget(0);
    terminal->setColorScheme("DarkPastels");
    ui->verticalLayout->addWidget(terminal);
    StartInstaller();
}

void KernelInstaller::StartInstaller()
{
    // 随机生成目录
    srand(time(0));
    int randomNumber = (rand() % (MAX_TMP_NUM - MIN_TMP_NUM)) + 1;  // 左闭右闭
    QString tempPath = tempDir + "/gxde-kernel-manager-installer-" + QString::number(randomNumber) + ".sh";
    QFile file(tempPath);
    file.open(QFile::WriteOnly);
    //file.write("#/bin/bash")
    file.close();
}

KernelInstaller::~KernelInstaller()
{
    delete ui;
}
