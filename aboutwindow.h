#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QMainWindow>

namespace Ui {
class AboutWindow;
}

class AboutWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow();

private slots:
    void on_m_closeButton_clicked();

    void on_m_logoShower_linkActivated(const QString &link);

private:
    Ui::AboutWindow *ui;
};

#endif // ABOUTWINDOW_H
