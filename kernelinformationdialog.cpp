#include "kernelinformationdialog.h"
#include "ui_kernelinformationdialog.h"

KernelInformationDialog::KernelInformationDialog(QJsonObject data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KernelInformationDialog)
{
    ui->setupUi(this);
    // 解析数据

}

KernelInformationDialog::~KernelInformationDialog()
{
    delete ui;
}
