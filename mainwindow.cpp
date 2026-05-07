#include "mainwindow.h"

#include <QWebEngineProfile>
#include <QWebEngineCertificateError>
#include <QDebug>

// ─── Configuration ────────────────────────────────────────────────────────────
// Adresse IP du Raspberry Pi sur le réseau local
static const QString RPI_IP   = "200.26.16.20";
static const QString WEBRTC_URL = QString("https://%1:8889/rascam").arg(RPI_IP);

// ─── Page personnalisée pour accepter le certificat auto-signé ────────────────
class CamWebPage : public QWebEnginePage
{
public:
    explicit CamWebPage(QObject *parent = nullptr)
        : QWebEnginePage(parent)
    {
        // Qt6 : accepte les certificats auto-signés via signal
        connect(this, &QWebEnginePage::certificateError,
                this, [](QWebEngineCertificateError error)
                {
                    qDebug() << "[TLS] Certificat auto-signé accepté :" << error.url();
                    error.acceptCertificate();
                });
    }
};

// ─── MainWindow ───────────────────────────────────────────────────────────────

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Camera – WebRTC sécurisé");
    resize(960, 600);

    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ── WebView ──────────────────────────────────────────────────────────────
    webView = new QWebEngineView(this);

    // Utilise notre page qui accepte le certificat auto-signé
    CamWebPage *page = new CamWebPage(webView);
    webView->setPage(page);

    // Active les permissions nécessaires à WebRTC
    auto *settings = webView->settings();
    settings->setAttribute(QWebEngineSettings::JavascriptEnabled,          true);
    settings->setAttribute(QWebEngineSettings::LocalStorageEnabled,        true);
    settings->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, false);

    // Autorise l'accès caméra/micro pour WebRTC (mediamtx en a besoin)
    connect(page, &QWebEnginePage::featurePermissionRequested,
            this, [page](const QUrl &origin, QWebEnginePage::Feature feature)
            {
                // On accorde toutes les permissions WebRTC
                page->setFeaturePermission(origin, feature,
                    QWebEnginePage::PermissionGrantedByUser);
            });

    connect(webView, &QWebEngineView::loadStarted, this, [this]
    {
        statusLabel->setText("Connexion en cours…");
    });

    connect(webView, &QWebEngineView::loadFinished, this, [this](bool ok)
    {
        statusLabel->setText(ok ? "✔ WebRTC connecté (chiffré)" : "⚠ Échec de connexion");
    });

    webView->setMinimumSize(640, 480);
    webView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    webView->load(QUrl(WEBRTC_URL));

    mainLayout->addWidget(webView, 1);

    // ── Barre de contrôle ────────────────────────────────────────────────────
    QWidget *controls = new QWidget(this);
    controls->setFixedHeight(44);
    controls->setStyleSheet("background-color: #1e1e1e;");
    QHBoxLayout *ctrlLayout = new QHBoxLayout(controls);
    ctrlLayout->setContentsMargins(8, 4, 8, 4);

    btnReload = new QPushButton("↺  Reconnecter", controls);
    statusLabel = new QLabel("En attente…", controls);
    statusLabel->setStyleSheet("color: #aaaaaa; font-size: 12px;");

    ctrlLayout->addWidget(btnReload);
    ctrlLayout->addStretch();
    ctrlLayout->addWidget(statusLabel);

    mainLayout->addWidget(controls);

    connect(btnReload, &QPushButton::clicked, this, &MainWindow::reloadStream);
}

void MainWindow::reloadStream()
{
    statusLabel->setText("Reconnexion…");
    webView->load(QUrl(WEBRTC_URL));
}
