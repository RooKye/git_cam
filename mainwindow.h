#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class QProcess;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void startMpv();

private:
    QWidget *videoWidget;
    QProcess *mpvProcess;
};

#endif // MAINWINDOW_H