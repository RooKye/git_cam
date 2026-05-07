#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QTimer>

class QMediaPlayer;
class QAudioOutput;
class QVideoWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startStream();
    void stopStream();

private:
    QVideoWidget *videoWidget  = nullptr;
    QMediaPlayer *player       = nullptr;
    QAudioOutput *audio        = nullptr;

    QLabel       *statusLabel  = nullptr;
    QPushButton  *btnStart     = nullptr;
    QPushButton  *btnStop      = nullptr;

    void setupUI();
};

#endif // MAINWINDOW_H
