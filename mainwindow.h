#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMediaPlayer;
class QAudioOutput;
class QVideoWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QMediaPlayer *player;
    QAudioOutput *audio;
    QVideoWidget *videoWidget;
};

#endif // MAINWINDOW_H