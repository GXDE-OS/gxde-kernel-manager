#ifndef KERNELINFORMATIONDIALOG_H
#define KERNELINFORMATIONDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class KernelInformationDialog;
}

class KernelInformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KernelInformationDialog(QJsonObject data, QWidget *parent = nullptr);
    ~KernelInformationDialog();

private:
    Ui::KernelInformationDialog *ui;
};

#endif // KERNELINFORMATIONDIALOG_H
