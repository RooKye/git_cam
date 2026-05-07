#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startStream();
    void stopStream();
    void onProcessError();
    void onReadyRead();

private:
    // Widget d'affichage vidéo GStreamer (window ID)
    QWidget     *videoContainer;
    QLabel      *statusLabel;
    QPushButton *btnStart;
    QPushButton *btnStop;

    QProcess    *gstProcess;

    void setupUI();
    void setupGStreamer();
};

#endif // MAINWINDOW_H
