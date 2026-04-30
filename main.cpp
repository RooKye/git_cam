#include <QApplication>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QVideoWidget video;
    video.resize(900, 600);
    video.setWindowTitle("Camera");

    QMediaPlayer player;
    QAudioOutput audio;

    player.setVideoOutput(&video);
    player.setAudioOutput(&audio);

    QObject::connect(&player, &QMediaPlayer::errorOccurred,
                     [](QMediaPlayer::Error error, const QString &errorString)
                     {
                         qDebug() << "Erreur Qt Multimedia :" << error << errorString;
                     });

    QObject::connect(&player, &QMediaPlayer::mediaStatusChanged,
                     [](QMediaPlayer::MediaStatus status)
                     {
                         qDebug() << "Statut média :" << status;
                     });

    player.setSource(QUrl("rtsp://127.0.0.1:8554/rascam"));
    player.play();

    video.show();

    return app.exec();
}