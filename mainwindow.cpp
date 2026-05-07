#include "mainwindow.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVideoWidget>
#include <QUrl>
#include <QDebug>

// URL du flux RTSP
static const QString RTSP_URL = "rtsp://127.0.0.1:8554/rascam";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Camera – Flux RTSP");
    resize(960, 600);

    // Force le backend GStreamer de Qt Multimedia avec pipeline basse latence.
    // Ces variables doivent être définies AVANT la création de QMediaPlayer.
    //
    // Le pipeline "playbin" de Qt utilise GStreamer en interne.
    // GST_PLAY_FLAG_VIDEO (1) + GST_PLAY_FLAG_NATIVE_VIDEO (64) = 65
    // On désactive l'audio (pas de flag audio) pour éviter tout buffer audio.
    qputenv("QT_MEDIA_BACKEND", "gstreamer");
    qputenv("GST_RTSP_TRANSPORT", "tcp");

    setupUI();
    startStream();
}

MainWindow::~MainWindow()
{
    stopStream();
}

// ─── UI ──────────────────────────────────────────────────────────────────────

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Zone vidéo gérée par Qt (QVideoWidget s'occupe du rendu)
    videoWidget = new QVideoWidget(this);
    videoWidget->setMinimumSize(640, 480);
    videoWidget->setStyleSheet("background-color: black;");
    videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(videoWidget, 1);

    // Barre de contrôle
    QWidget *controls = new QWidget(this);
    controls->setFixedHeight(44);
    controls->setStyleSheet("background-color: #1e1e1e;");
    QHBoxLayout *ctrlLayout = new QHBoxLayout(controls);
    ctrlLayout->setContentsMargins(8, 4, 8, 4);

    btnStart = new QPushButton("▶  Démarrer", controls);
    btnStop  = new QPushButton("■  Arrêter",  controls);
    statusLabel = new QLabel("En attente…", controls);
    statusLabel->setStyleSheet("color: #aaaaaa; font-size: 12px;");

    btnStart->setEnabled(false);
    btnStop->setEnabled(true);

    ctrlLayout->addWidget(btnStart);
    ctrlLayout->addWidget(btnStop);
    ctrlLayout->addStretch();
    ctrlLayout->addWidget(statusLabel);

    mainLayout->addWidget(controls);

    connect(btnStart, &QPushButton::clicked, this, &MainWindow::startStream);
    connect(btnStop,  &QPushButton::clicked, this, &MainWindow::stopStream);
}

// ─── Lecteur ─────────────────────────────────────────────────────────────────

void MainWindow::startStream()
{
    if (player)
        return;

    player = new QMediaPlayer(this);
    audio  = new QAudioOutput(this);

    player->setVideoOutput(videoWidget);
    player->setAudioOutput(audio);
    audio->setVolume(0);   // flux vidéo uniquement, pas d'audio

    // Signaux de diagnostic
    connect(player, &QMediaPlayer::errorOccurred,
            this, [this](QMediaPlayer::Error /*err*/, const QString &msg)
            {
                statusLabel->setText("⚠ " + msg);
                qWarning() << "[Player] Erreur :" << msg;
            });

    connect(player, &QMediaPlayer::mediaStatusChanged,
            this, [this](QMediaPlayer::MediaStatus status)
            {
                switch (status)
                {
                case QMediaPlayer::LoadingMedia:
                    statusLabel->setText("Chargement…");         break;
                case QMediaPlayer::BufferingMedia:
                    statusLabel->setText("Mise en mémoire tampon…"); break;
                case QMediaPlayer::BufferedMedia:
                    statusLabel->setText("✔ Flux en lecture");   break;
                case QMediaPlayer::EndOfMedia:
                case QMediaPlayer::InvalidMedia:
                    statusLabel->setText("Flux perdu – relance…");
                    // Reconnexion automatique après 2 s
                    QTimer::singleShot(2000, this, [this]{ player->play(); });
                    break;
                default: break;
                }
                qDebug() << "[Player] Statut :" << status;
            });

    player->setSource(QUrl(RTSP_URL));
    player->play();

    btnStart->setEnabled(false);
    btnStop->setEnabled(true);
    statusLabel->setText("Connexion au flux…");
}

void MainWindow::stopStream()
{
    if (!player)
        return;

    player->stop();
    delete player;  player = nullptr;
    delete audio;   audio  = nullptr;

    statusLabel->setText("Flux arrêté");
    btnStart->setEnabled(true);
    btnStop->setEnabled(false);
}
