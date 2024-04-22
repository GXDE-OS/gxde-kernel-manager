#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "kernelinformation.h"

#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    RefreshKernelList();
}

void MainWindow::RefreshKernelList()
{
    KernelInformation *information = new KernelInformation();
    connect(information, &KernelInformation::loadFinished, this, [this, information](){
        qDebug() << information->get_listData();
        RefreshKernelListView(information);
        delete information;
    });
    information->LoadInfo();
}

void MainWindow::RefreshKernelListView(KernelInformation *info)
{
    // 更新列表
    int count = info->get_count();
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(QStringList() << tr("Kernel Name") << tr("Author"));
    for(int i = 0; i < count; i++) {
        model->setItem(i, 0, new QStandardItem(info->get_name(i)));
        model->setItem(i, 1, new QStandardItem(info->get_author(i)));
    }
    ui->m_kernelShow->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

