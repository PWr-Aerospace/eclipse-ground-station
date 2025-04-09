#include "MainWindow.h"
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent),
      discoveryPanel(new DiscoveryPanel(this)),
      videoWidget(new VideoWidget(this)),
      tabWidget(new QTabWidget(this))
{
    setupUI();
    setupConnections();
    resize(1000, 700); 
}


void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    QWidget *topPanel = new QWidget();
    QHBoxLayout *topLayout = new QHBoxLayout(topPanel);
    
    topLayout->addStretch(); 
    topLayout->addWidget(discoveryPanel);
    topLayout->setContentsMargins(0, 0, 0, 10);
    
    mainLayout->addWidget(topPanel);
    mainLayout->addWidget(tabWidget, 1); 
    
    tabWidget->addTab(videoWidget, "Video Stream");
    tabWidget->addTab(new QWidget(), "Controls");
    
    setCentralWidget(centralWidget);
}

void MainWindow::setupConnections() {
    connect(discoveryPanel, &DiscoveryPanel::connectClicked, [this]() {
        videoWidget->startStreaming(discoveryPanel->currentDevicePort());
    });
    
    connect(videoWidget, &VideoWidget::connectionStatusChanged,
            discoveryPanel, &DiscoveryPanel::updateConnectionStatus);
}