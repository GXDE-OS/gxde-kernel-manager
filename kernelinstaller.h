#ifndef KERNELINSTALLER_H
#define KERNELINSTALLER_H

#include <QMainWindow>

#include <qtermwidget5/qtermwidget.h>

namespace Ui {
class KernelInstaller;
}

class KernelInstaller : public QMainWindow
{
    Q_OBJECT

public:
    explicit KernelInstaller(QStringList kernelList, QWidget *parent = nullptr);
    ~KernelInstaller();

signals:
    void InstallFinished(int status);

private:
    Ui::KernelInstaller *ui;
    QTermWidget *terminal;
    QStringList kernelList;
    int processID;

    QString tempDir = "/tmp";

    void StartInstaller();
    QString BuildKernelInstallerBash(QStringList kernelList, QString tempFilePath) const;
    QString BuildKernelRootLoaderInstallerBash(QString tempFilePath, QString tempLoaderFilePath) const;

    int CheckInstallerStatus();
    void CheckInstallerStatusTimer();
};

#endif // KERNELINSTALLER_H
