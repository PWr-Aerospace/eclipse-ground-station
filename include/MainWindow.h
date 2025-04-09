#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DiscoveryPanel.h"
#include "VideoWidget.h"

class QTabWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupConnections();

    DiscoveryPanel *discoveryPanel;
    VideoWidget *videoWidget;
    QTabWidget *tabWidget;
    QLabel *logoLabel;
};

#endif // MAINWINDOW_H