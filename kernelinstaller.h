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

private:
    Ui::KernelInstaller *ui;
    QTermWidget *terminal;
    QStringList kernelList;

    QString tempDir = "/tmp";

    void StartInstaller();
};

#endif // KERNELINSTALLER_H
