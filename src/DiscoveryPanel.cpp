#include "DiscoveryPanel.h"
#include "DiscoveryService.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

DiscoveryPanel::DiscoveryPanel(QWidget *parent) : QWidget(parent) {
    discoveryService = new DiscoveryService(this);
    setupUI();
    
    connect(discoveryService, &DiscoveryService::deviceDiscovered, 
            this, &DiscoveryPanel::onDeviceDiscovered);
    connect(connectButton, &QPushButton::clicked, 
            this, &DiscoveryPanel::connectClicked);
    connect(refreshButton, &QPushButton::clicked, 
            this, &DiscoveryPanel::refreshClicked);
}

DiscoveryPanel::~DiscoveryPanel() {
    delete discoveryService;
}

void DiscoveryPanel::setupUI() {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(5);

    QWidget *container = new QWidget();
    QHBoxLayout *containerLayout = new QHBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->setSpacing(5);
    
    devicesCombo = new QComboBox(this);
    connectButton = new QPushButton(tr("Connect"), this);
    refreshButton = new QPushButton(tr("Refresh"), this);
    statusLabel = new QLabel(tr("Disconnected"), this);
    
    connectButton->setFixedWidth(80);
    refreshButton->setFixedWidth(80);
    devicesCombo->setMinimumWidth(200);
    
    connectButton->setEnabled(false);

    containerLayout->addWidget(devicesCombo);
    containerLayout->addWidget(connectButton);
    containerLayout->addWidget(refreshButton);
    containerLayout->addWidget(statusLabel);
    
    mainLayout->addStretch();
    mainLayout->addWidget(container);
    
    connect(devicesCombo,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        &DiscoveryPanel::onDeviceSelectionChanged);
    connect(connectButton, &QPushButton::clicked, this, &DiscoveryPanel::connectClicked);
    connect(refreshButton, &QPushButton::clicked, this, &DiscoveryPanel::refreshClicked);
}

void DiscoveryPanel::onDeviceDiscovered(const QString &address, quint16 port) {
    if (!discoveredDevices.contains(address)) {
        discoveredDevices.insert(address, port);
        devicesCombo->addItem(QString("Device @ %1").arg(address), address);
        
        statusLabel->setText(QString("Found %1 device(s)").arg(discoveredDevices.size()));
    }
}

QString DiscoveryPanel::currentDeviceAddress() const {
    if (devicesCombo->currentIndex() == -1) return QString();
    return devicesCombo->currentData().toString();
}

quint16 DiscoveryPanel::currentDevicePort() const {
    QString address = currentDeviceAddress();
    if (address.isEmpty()) return 0;
    return discoveredDevices.value(address);
}
void DiscoveryPanel::onDeviceSelectionChanged(int index)
{
    connectButton->setEnabled(index >= 0 && !isConnected);
}
void DiscoveryPanel::updateConnectionStatus(bool connected) {
    isConnected = connected;
    connectButton->setEnabled(!connected && devicesCombo->count() > 0);
    devicesCombo->setEnabled(!connected);
    refreshButton->setEnabled(!connected);
    
    if (connected) {
        statusLabel->setText(QString("Connected to %1").arg(devicesCombo->currentText()));
    } else {
        statusLabel->setText(QString("Found %1 device(s)").arg(discoveredDevices.size()));
    }
}