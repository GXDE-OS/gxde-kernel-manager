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

signals:
    void InstallFinished(int status);
    void RemoveFinished(int status);

private slots:
    void on_m_refreshButton_clicked();

    void on_m_reconfigureButton_clicked();

    void on_m_installButton_clicked();

    void on_m_removeButton_clicked();

private:
    Ui::KernelInformationDialog *ui;
    QStringList pkgList;
};

#endif // KERNELINFORMATIONDIALOG_H
