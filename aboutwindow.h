#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMouseEvent>
#include <QUrl>

// 处理鼠标点击
class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    using QLabel::QLabel;

signals:
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *ev) override {
        if (ev->button() == Qt::LeftButton)
            emit clicked();
        QLabel::mouseReleaseEvent(ev);
    }
};

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

    // void on_m_logoShower_linkActivated(const QString &link);

private:
    Ui::AboutWindow *ui;
};

#endif // ABOUTWINDOW_H
