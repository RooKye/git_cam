#include "mainwindow.h"

#include <QDebug>
#include <QStatusBar>

// URL du flux RTSP
static const QString RTSP_URL = "rtsp://127.0.0.1:8554/rascam";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , gstProcess(nullptr)
{
    setWindowTitle("Camera – Flux RTSP");
    resize(960, 600);

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

    // Zone vidéo : fond noir, GStreamer y dessine directement
    videoContainer = new QWidget(this);
    videoContainer->setMinimumSize(640, 480);
    videoContainer->setStyleSheet("background-color: black;");
    videoContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // Empêche Qt de redessiner par-dessus GStreamer
    videoContainer->setAttribute(Qt::WA_NativeWindow);
    videoContainer->setAttribute(Qt::WA_PaintOnScreen);
    mainLayout->addWidget(videoContainer, 1);

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

    btnStart->setEnabled(false);   // déjà lancé au démarrage
    btnStop->setEnabled(true);

    ctrlLayout->addWidget(btnStart);
    ctrlLayout->addWidget(btnStop);
    ctrlLayout->addStretch();
    ctrlLayout->addWidget(statusLabel);

    mainLayout->addWidget(controls);

    connect(btnStart, &QPushButton::clicked, this, &MainWindow::startStream);
    connect(btnStop,  &QPushButton::clicked, this, &MainWindow::stopStream);
}

// ─── GStreamer ────────────────────────────────────────────────────────────────

void MainWindow::setupGStreamer()
{
    gstProcess = new QProcess(this);

    connect(gstProcess, &QProcess::readyReadStandardError,
            this, &MainWindow::onReadyRead);
    connect(gstProcess, &QProcess::errorOccurred,
            this, &MainWindow::onProcessError);
    connect(gstProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int code, QProcess::ExitStatus)
            {
                qDebug() << "GStreamer terminé, code :" << code;
                statusLabel->setText("Flux arrêté");
                btnStart->setEnabled(true);
                btnStop->setEnabled(false);
            });
}

void MainWindow::startStream()
{
    if (gstProcess && gstProcess->state() != QProcess::NotRunning)
        return;

    delete gstProcess;
    setupGStreamer();

    // Récupère le WinId natif pour que GStreamer dessine dans notre widget
    WId winId = videoContainer->winId();

    /*
     * Pipeline basse latence :
     *  - rtspsrc        : source RTSP, latency=0, forçage TCP
     *  - rtph264depay   : dépaquetage RTP → H.264 NAL
     *  - h264parse      : parsing H.264
     *  - avdec_h264     : décodage logiciel (remplacer par nvh264dec si GPU NVIDIA)
     *  - videoconvert   : conversion colorimétrique
     *  - ximagesink     : rendu dans la fenêtre Qt (sync=false = pas de resync horloge)
     */
    QStringList args;
    args << "-v"
         << "rtspsrc"
              << QString("location=%1").arg(RTSP_URL)
              << "latency=0"
              << "protocols=tcp"
              << "buffer-mode=0"       // pas de buffer de jitter côté récepteur
         << "!"
         << "rtph264depay"
         << "!"
         << "h264parse"
         << "!"
         << "avdec_h264"
              << "skip-frame=0"        // ne saute aucune frame
         << "!"
         << "videoconvert"
         << "!"
         << "ximagesink"
              << "sync=false"          // désactive la synchro horloge → latence minimale
              << QString("window-handle=%1").arg(winId);

    qDebug() << "Lancement GStreamer :" << "gst-launch-1.0" << args.join(" ");

    gstProcess->start("gst-launch-1.0", args);

    if (!gstProcess->waitForStarted(3000))
    {
        statusLabel->setText("⚠ Impossible de lancer gst-launch-1.0");
        qWarning() << "gst-launch-1.0 introuvable ou timeout";
        return;
    }

    statusLabel->setText("Connexion au flux…");
    btnStart->setEnabled(false);
    btnStop->setEnabled(true);
}

void MainWindow::stopStream()
{
    if (!gstProcess)
        return;

    if (gstProcess->state() != QProcess::NotRunning)
    {
        gstProcess->terminate();
        if (!gstProcess->waitForFinished(2000))
            gstProcess->kill();
    }

    statusLabel->setText("Flux arrêté");
    btnStart->setEnabled(true);
    btnStop->setEnabled(false);
}

void MainWindow::onProcessError()
{
    statusLabel->setText("⚠ Erreur GStreamer : " + gstProcess->errorString());
    qWarning() << "Erreur process GStreamer :" << gstProcess->errorString();
}

void MainWindow::onReadyRead()
{
    // Redirige stderr de GStreamer vers la console Qt
    const QByteArray output = gstProcess->readAllStandardError();
    const QString line = QString::fromUtf8(output).trimmed();

    // Détecte "Playing" dans les logs pour mettre à jour le statut
    if (line.contains("PLAYING", Qt::CaseInsensitive))
        statusLabel->setText("✔ Flux en lecture");

    qDebug() << "[GST]" << line;
}
