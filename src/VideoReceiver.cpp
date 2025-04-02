#include "VideoReceiver.h"
#include "DiscoveryService.h"
#include "VideoStream.h"

VideoReceiver::VideoReceiver(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    
    discoveryService = new DiscoveryService(this);
    videoStream = new VideoStream(this);
    
    connect(discoveryService, &DiscoveryService::deviceDiscovered, 
            this, &VideoReceiver::onDeviceDiscovered);
    connect(videoStream, &VideoStream::imageReceived, 
            this, &VideoReceiver::onImageReceived);
    connect(videoStream, &VideoStream::connectionStatusChanged, 
            this, &VideoReceiver::onConnectionStatusChanged);
    connect(connectButton, &QPushButton::clicked,
            this, &VideoReceiver::connectToSelectedDevice);
    connect(refreshButton, &QPushButton::clicked,
            this, &VideoReceiver::clearDevicesList);
}

VideoReceiver::~VideoReceiver() {
    delete discoveryService;
    delete videoStream;
}

void VideoReceiver::setupUI() {
    setWindowTitle("Eclipse");
    resize(800, 600);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setText("Waiting for video stream...");
    imageLabel->setStyleSheet("QLabel { background-color: black; color: white; }");

    statusLabel = new QLabel("Searching for devices...", this);
    statusLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *selectionLayout = new QHBoxLayout();
    devicesCombo = new QComboBox(this);
    devicesCombo->setPlaceholderText("Select  device");
    
    connectButton = new QPushButton("Connect", this);
    connectButton->setEnabled(false);
    
    refreshButton = new QPushButton("Refresh", this);
    
    selectionLayout->addWidget(devicesCombo, 1);
    selectionLayout->addWidget(connectButton);
    selectionLayout->addWidget(refreshButton);

    mainLayout->addWidget(imageLabel, 1);
    mainLayout->addLayout(selectionLayout);
    mainLayout->addWidget(statusLabel);

    setCentralWidget(centralWidget);
}

void VideoReceiver::onDeviceDiscovered(const QString &address, quint16 port) {
    if (!discoveredDevices.contains(address)) {
        discoveredDevices.insert(address, port);
        devicesCombo->addItem(QString("Device @ %1").arg(address), address);
        
        if (devicesCombo->count() == 1) {
            connectButton->setEnabled(true);
        }
        
        statusLabel->setText(QString("Found %1 device(s)").arg(discoveredDevices.size()));
    }
}

void VideoReceiver::connectToSelectedDevice() {
    if (devicesCombo->currentIndex() == -1) return;
    
    QString address = devicesCombo->currentData().toString();
    quint16 port = discoveredDevices.value(address);
    
    statusLabel->setText(QString("Connecting to %1...").arg(address));
    videoStream->startStreaming(port);
}

void VideoReceiver::onImageReceived(const QImage &image) {
    imageLabel->setPixmap(QPixmap::fromImage(image).scaled(
        imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    statusLabel->setText(QString("Connected to %1").arg(devicesCombo->currentText()));
}

void VideoReceiver::onConnectionStatusChanged(bool connected) {
    if (!connected) {
        statusLabel->setText("Connection lost");
        imageLabel->setText("No video stream...");
    }
}

void VideoReceiver::clearDevicesList() {
    discoveredDevices.clear();
    devicesCombo->clear();
    connectButton->setEnabled(false);
    statusLabel->setText("Searching for device...");
}