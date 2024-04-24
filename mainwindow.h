#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "kernelinformation.h"
#include "kernelinstaller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_m_refreshButton_clicked();

    void on_m_installButton_clicked();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionGitee_triggered();

    void on_actionGithub_triggered();

private:
    Ui::MainWindow *ui;
    KernelInformation *kernelInformation;
    void RefreshKernelList();
    void RefreshKernelListView(KernelInformation *info);
};
#endif // MAINWINDOW_H
