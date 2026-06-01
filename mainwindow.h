#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <vlc/vlc.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *videoWidget;

    libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *mediaPlayer;
};

#endif // MAINWINDOW_H