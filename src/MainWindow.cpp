#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QTextEdit>
#include "GamepadController.h"

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
    logger = new LoggerWidget(this);
    tabWidget->addTab(videoWidget, "Video Stream");
    tabWidget->addTab(new QWidget(), "Controls");
    tabWidget->addTab(logger, "Logs");


    gamepadController = new GamepadController("xxxx", 0000, this);
    connect(gamepadController, &GamepadController::logMessage,
            this, &MainWindow::appendLog);
    
    setCentralWidget(centralWidget);
}

void MainWindow::setupConnections() {
    connect(discoveryPanel, &DiscoveryPanel::connectClicked, [this]() {
        videoWidget->startStreaming(discoveryPanel->currentDevicePort());
    });
    
    connect(videoWidget, &VideoWidget::connectionStatusChanged,
            discoveryPanel, &DiscoveryPanel::updateConnectionStatus);

    connect(gamepadController, &GamepadController::logMessage,
        logger, &LoggerWidget::appendLog);
        
}

void MainWindow::appendLog(const QString &text)
{
    if (logger)
        logger->appendLog(text);
}


