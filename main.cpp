#include <QApplication>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QUrl>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QVideoWidget videoWidget;
    videoWidget.resize(900, 600);
    videoWidget.setWindowTitle("Camera");

    QMediaPlayer player;
    player.setVideoOutput(&videoWidget);

    player.setSource(QUrl("rtsp://127.0.0.1:8554/rascam"));
    player.play();

    videoWidget.show();

    return app.exec();
}