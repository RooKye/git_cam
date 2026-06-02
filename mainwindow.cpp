#include "mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    videoWidget(nullptr),
    vlcInstance(nullptr),
    mediaPlayer(nullptr)
{
    setWindowTitle("Caméra RTSP");
    resize(1000, 600);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);

    videoWidget = new QWidget(this);
    videoWidget->setStyleSheet("background-color: black;");
    videoWidget->setAttribute(Qt::WA_NativeWindow);

    layout->addWidget(videoWidget);

    const char *vlcArgs[] = {
        "--no-video-title-show"
    };

    vlcInstance = libvlc_new(1, vlcArgs);
    mediaPlayer = libvlc_media_player_new(vlcInstance);

    WId id = videoWidget->winId();
#include "mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QProcess>
#include <QTimer>

    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
        videoWidget(nullptr),
        mpvProcess(nullptr)
    {
        setWindowTitle("Caméra RTSP");
        resize(1000, 600);

        QWidget *central = new QWidget(this);
        setCentralWidget(central);

        QVBoxLayout *layout = new QVBoxLayout(central);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        videoWidget = new QWidget(this);
        videoWidget->setStyleSheet("background-color: black;");

        // Important pour que mpv puisse s'intégrer dans le widget Qt
        videoWidget->setAttribute(Qt::WA_NativeWindow);
        videoWidget->setAttribute(Qt::WA_DontCreateNativeAncestors);

        layout->addWidget(videoWidget);

        mpvProcess = new QProcess(this);
        mpvProcess->setProcessChannelMode(QProcess::ForwardedChannels);

        // On attend que la fenêtre Qt soit bien créée avant d'injecter mpv dedans
        QTimer::singleShot(300, this, &MainWindow::startMpv);
    }

    MainWindow::~MainWindow()
    {
        if (mpvProcess) {
            mpvProcess->terminate();

            if (!mpvProcess->waitForFinished(1000)) {
                mpvProcess->kill();
            }
        }
    }

    void MainWindow::startMpv()
    {
        QString url = "rtsp://127.0.0.1:8554/rascam";
        QString windowId = QString::number(static_cast<qulonglong>(videoWidget->winId()));

        QStringList args;

        args << "--no-terminal";
        args << "--no-osc";
        args << "--no-border";
        args << "--force-window=yes";

        // Le plus important pour ton tunnel : forcer RTSP en TCP
        args << "--demuxer-lavf-o=rtsp_transport=tcp";

        // Réduit la latence
        args << "--profile=low-latency";

        // Intégration de mpv dans le QWidget
        args << "--wid=" + windowId;

        // Flux vidéo
        args << url;

        mpvProcess->start("mpv", args);
    }
#ifdef Q_OS_WIN
    libvlc_media_player_set_hwnd(mediaPlayer, reinterpret_cast<void *>(id));
#elif defined(Q_OS_MAC)
    libvlc_media_player_set_nsobject(mediaPlayer, reinterpret_cast<void *>(id));
#else
    libvlc_media_player_set_xwindow(mediaPlayer, static_cast<uint32_t>(id));
#endif

    const char *url = "rtsp://127.0.0.1:8554/rascam";

    libvlc_media_t *media = libvlc_media_new_location(vlcInstance, url);

    libvlc_media_add_option(media, ":rtsp-tcp");
    libvlc_media_add_option(media, ":network-caching=150");

    libvlc_media_player_set_media(mediaPlayer, media);
    libvlc_media_release(media);

    libvlc_media_player_play(mediaPlayer);
}

MainWindow::~MainWindow()
{
    libvlc_media_player_stop(mediaPlayer);
    libvlc_media_player_release(mediaPlayer);
    libvlc_release(vlcInstance);
}