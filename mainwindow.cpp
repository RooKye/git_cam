#include "mainwindow.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVideoWidget>
#include <QUrl>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Camera");
    resize(900, 600);

    videoWidget = new QVideoWidget(this);
    player = new QMediaPlayer(this);
    audio = new QAudioOutput(this);

    setCentralWidget(videoWidget);

    player->setVideoOutput(videoWidget);
    player->setAudioOutput(audio);

    QObject::connect(player, &QMediaPlayer::errorOccurred,
                     this, [](QMediaPlayer::Error error, const QString &errorString)
                     {
                         qDebug() << "Erreur Qt Multimedia :" << error << errorString;
                     });

    QObject::connect(player, &QMediaPlayer::mediaStatusChanged,
                     this, [](QMediaPlayer::MediaStatus status)
                     {
                         qDebug() << "Statut média :" << status;
                     });

    player->setSource(QUrl("rtsp://127.0.0.1:8554/rascam"));
    player->play();
}

MainWindow::~MainWindow()
{
    player->stop();
}