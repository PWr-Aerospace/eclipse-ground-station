#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DiscoveryPanel.h"
#include "VideoWidget.h"
#include "GamepadController.h"
#include <QTextEdit>
#include "LoggerWidget.h"


class QTabWidget;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupConnections();
    void appendLog(const QString &text);

    DiscoveryPanel *discoveryPanel;
    VideoWidget *videoWidget;
    QTabWidget *tabWidget;
    QLabel *logoLabel;
    LoggerWidget *logger;

    GamepadController *gamepadController = nullptr; 
};

#endif // MAINWINDOW_H
