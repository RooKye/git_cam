#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineSettings>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void reloadStream();

private:
    QWebEngineView *webView    = nullptr;
    QLabel         *statusLabel = nullptr;
    QPushButton    *btnReload  = nullptr;

    void setupUI();
};

#endif // MAINWINDOW_H
