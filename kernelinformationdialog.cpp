#include "kernelinformationdialog.h"
#include "ui_kernelinformationdialog.h"

#include <QJsonArray>

KernelInformationDialog::KernelInformationDialog(QJsonObject data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KernelInformationDialog)
{
    ui->setupUi(this);
    // 解析数据
    ui->m_kernelName->setText(tr("Kernel Name:") + " " + data.value("Name").toString());
    ui->m_kernelVersion->setText(tr("Kernel Version:") + " " + data.value("Ver").toString());
    ui->m_kernelAuthor->setText(tr("Author:") + " " + data.value("Author").toString());
    QJsonArray array = data.value("PkgName").toArray();
    QString kernelText = "";
    for(QJsonValue i: array) {
        kernelText += i.toString() + " ";
    }
    ui->m_PkgName->setText(tr("Package Name:") + " " + kernelText);
    ui->m_kernelArch->setText(tr("Kernel Architecture:") + " " + data.value("Arch").toArray().at(0).toString());
    ui->m_des->setText(data.value("Des").toString());
}

KernelInformationDialog::~KernelInformationDialog()
{
    delete ui;
}
