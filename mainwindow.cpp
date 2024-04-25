#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"

#include "kernelinformation.h"

#include <QStandardItemModel>
#include <qdesktopservices.h>

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
    ui->m_nowKernel->setText(tr("Kernel: ") + kernelInformation->localKernelName() + " " + tr("Arch: ") + kernelInformation->arch());
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
    model->setHorizontalHeaderLabels(QStringList() << tr("ID") << tr("Kernel Name") << tr("Author") << tr("Arch") << tr("Installed"));
    for(int i = 0; i < count; i++) {
        model->setItem(i, 0, new QStandardItem(QString::number(i)));
        model->setItem(i, 1, new QStandardItem(info->get_name(i)));
        model->setItem(i, 2, new QStandardItem(info->get_author(i)));
        model->setItem(i, 3, new QStandardItem(info->get_arch(i).at(0)));
        model->setItem(i, 4, new QStandardItem((QStringList() << "" << "Y").at(info->get_installedAlready(i))));
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
    QModelIndex list = ui->m_kernelShow->selectionModel()->currentIndex();
    int row = list.row();
    if(row <= 0) {
        // 未选中任何内容
        QMessageBox::critical(this, tr("Error"), tr("Nothing to choose"));
        return;
    }
    // 获取 ID
    QModelIndex index = ui->m_kernelShow->model()->index(row, 0);
    int id = ui->m_kernelShow->model()->data(index).toUInt();
    // 获取选中行
    KernelInstaller *installer = new KernelInstaller(KernelInstaller::Option::Install, kernelInformation->get_pkgName(id));
    installer->show();
}


void MainWindow::on_actionExit_triggered()
{
    this->close();
}


void MainWindow::on_actionAbout_triggered()
{
    AboutWindow *aboutWindow = new AboutWindow();
    aboutWindow->show();
}


void MainWindow::on_actionGitee_triggered()
{
    QDesktopServices::openUrl(QUrl("https://gitee.com/GXDE-OS/gxde-kernel-manager"));
}


void MainWindow::on_actionGithub_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/GXDE-OS/gxde-kernel-manager"));
}


void MainWindow::on_m_removeButton_clicked()
{
    QModelIndex list = ui->m_kernelShow->selectionModel()->currentIndex();
    int row = list.row();
    if(row <= 0) {
        // 未选中任何内容
        QMessageBox::critical(this, tr("Error"), tr("Nothing to choose"));
        return;
    }
    // 获取 ID
    QModelIndex index = ui->m_kernelShow->model()->index(row, 0);
    int id = ui->m_kernelShow->model()->data(index).toUInt();
    // 获取选中行
    KernelInstaller *installer = new KernelInstaller(KernelInstaller::Option::Remove, kernelInformation->get_pkgName(id));
    installer->show();
}

