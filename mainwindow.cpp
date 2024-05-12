#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "programinfo.h"

#include "kernelinformation.h"

#include <QStandardItemModel>
#include <qdesktopservices.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(this->windowTitle() + " " + ProgramInfo::version());
    kernelInformation = new KernelInformation();
    RefreshKernelList();
}

void MainWindow::RefreshKernelList()
{
    ui->m_nowKernel->setText(tr("Kernel: ") + kernelInformation->localKernelName() + " " + tr("Arch: ") + kernelInformation->arch());
    connect(kernelInformation, &KernelInformation::loadFinished, this, [this](){
        RefreshKernelListView(kernelInformation, ui->m_showLocalArchOnly->isChecked());
    });
    kernelInformation->LoadInfo();
}

void MainWindow::RefreshKernelListView(KernelInformation *info, bool showLocalArchOnly)
{
    // 更新列表
    int count = info->get_count();
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(QStringList() << tr("ID") << tr("Kernel Name") << tr("Author") << tr("Arch") << tr("Installed") << tr("Description"));
    const QString arch = info->arch();
    int line = 0;
    for(int i = 0; i < count; i++) {
        // 显示所有架构
        QString kernelArch = "";
        bool isLocalArch = false;

        for(QString i: info->get_arch(i)) {
            if(i == arch) {
                isLocalArch = true;
            }
            if(i == "all") {
                isLocalArch = true;
            }
            kernelArch += i + " ";
        }
        if(showLocalArchOnly && !isLocalArch) {
            continue;
        }
        model->setItem(line, 0, new QStandardItem(QString::number(i)));
        model->setItem(line, 1, new QStandardItem(info->get_name(i)));
        model->setItem(line, 2, new QStandardItem(info->get_author(i)));
        model->setItem(line, 3, new QStandardItem(kernelArch));
        model->setItem(line, 4, new QStandardItem((QStringList() << "" << "Y").at(info->get_installedAlready(i))));
        model->setItem(line, 5, new QStandardItem(info->get_des(i)));
        line++;
    }
    ui->m_kernelShow->setModel(model);
    ui->m_kernelShow->resizeColumnsToContents();
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
    if(row < 0) {
        // 未选中任何内容
        QMessageBox::critical(this, tr("Error"), tr("Nothing to choose"));
        return;
    }
    // 获取 ID
    QModelIndex index = ui->m_kernelShow->model()->index(row, 0);
    int id = ui->m_kernelShow->model()->data(index).toUInt();
    // 获取选中行
    KernelInstaller *installer = new KernelInstaller(KernelInstaller::Option::Install, kernelInformation->get_pkgName(id));
    connect(installer, &KernelInstaller::InstallFinished, this, [this](){
        // 刷新列表
        this->RefreshKernelListView(this->kernelInformation, ui->m_showLocalArchOnly->isChecked());
    });
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
    if(row < 0) {
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


void MainWindow::on_m_showLocalArchOnly_stateChanged(int arg1)
{
    RefreshKernelListView(this->kernelInformation, ui->m_showLocalArchOnly->isChecked());
}


void MainWindow::on_m_reconfigureButton_clicked()
{
    QModelIndex list = ui->m_kernelShow->selectionModel()->currentIndex();
    int row = list.row();
    if(row < 0) {
        // 未选中任何内容
        QMessageBox::critical(this, tr("Error"), tr("Nothing to choose"));
        return;
    }
    // 获取 ID
    QModelIndex index = ui->m_kernelShow->model()->index(row, 0);
    int id = ui->m_kernelShow->model()->data(index).toUInt();
    // 获取选中行
    KernelInstaller *installer = new KernelInstaller(KernelInstaller::Option::Reconfigure, kernelInformation->get_pkgName(id));
    installer->show();
}


void MainWindow::on_actionUpdate_apt_cache_triggered()
{
    KernelInstaller *installer = new KernelInstaller(KernelInstaller::Option::Update, QStringList());
    connect(installer, &KernelInstaller::InstallFinished, this, [this, installer](){
        RefreshKernelList();
    });
    installer->show();
}

