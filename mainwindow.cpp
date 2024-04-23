#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "kernelinformation.h"

#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    kernelInformation = new KernelInformation();
    RefreshKernelList();
}

void MainWindow::RefreshKernelList()
{
    ui->m_nowKernel->setText(tr("Kernel: ") + kernelInformation->localKernelName());
    connect(kernelInformation, &KernelInformation::loadFinished, this, [this](){
        qDebug() << this->kernelInformation->get_listData();
        RefreshKernelListView(kernelInformation);
    });
    kernelInformation->LoadInfo();
}

void MainWindow::RefreshKernelListView(KernelInformation *info)
{
    // 更新列表
    int count = info->get_count();
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(QStringList() << tr("ID") << tr("Kernel Name") << tr("Author"));
    for(int i = 0; i < count; i++) {
        model->setItem(i, 0, new QStandardItem(QString::number(i)));
        model->setItem(i, 1, new QStandardItem(info->get_name(i)));
        model->setItem(i, 2, new QStandardItem(info->get_author(i)));
    }
    ui->m_kernelShow->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_m_refreshButton_clicked()
{
    RefreshKernelList();
}


void MainWindow::on_m_installButton_clicked()
{
    KernelInstaller *installer = new KernelInstaller(kernelInformation->get_pkgName(0));
    installer->show();
}

