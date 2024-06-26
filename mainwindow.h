#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
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

    void on_m_removeButton_clicked();

    void on_m_showLocalArchOnly_stateChanged(int arg1);

    void on_m_reconfigureButton_clicked();

    void on_actionUpdate_apt_cache_triggered();

    void on_actionAbout_QT_triggered();

    void on_actionUpgrade_triggered();

    void on_actionDonate_triggered();

    void on_m_kernelShow_doubleClicked(const QModelIndex &index);

    void on_actionSourceforge_triggered();

private:
    Ui::MainWindow *ui;
    KernelInformation *kernelInformation;
    void RefreshKernelList();
    void RefreshKernelListView(KernelInformation *info, bool showLocalArchOnly = true);

    int kernelNumber = 0;
};
#endif // MAINWINDOW_H
