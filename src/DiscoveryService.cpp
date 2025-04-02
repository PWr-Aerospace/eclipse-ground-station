#include "DiscoveryService.h"
#include <QNetworkDatagram>

DiscoveryService::DiscoveryService(QObject *parent) : QObject(parent) {
    discoverySocket = new QUdpSocket(this);
    if (!discoverySocket->bind(5001, QUdpSocket::ShareAddress)) {
        qWarning() << "Could not bind to discovery port 5001";
    }
    connect(discoverySocket, &QUdpSocket::readyRead, this, &DiscoveryService::processDiscoveryDatagrams);
}

void DiscoveryService::processDiscoveryDatagrams() {
    while (discoverySocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = discoverySocket->receiveDatagram();
        QByteArray data = datagram.data();

        if (data.startsWith("RPI_VIDEO_STREAM:")) {
            QStringList parts = QString(data).split(':');
            if (parts.size() >= 3) {
                QString address = parts[1];
                quint16 port = parts[2].toUShort();
                emit deviceDiscovered(address, port);
                startDiscovery(datagram.senderAddress());
            }
        }
    }
}

void DiscoveryService::startDiscovery(const QHostAddress &senderAddress) {
    QByteArray response = "PC_RESPONSE";
    discoverySocket->writeDatagram(response, senderAddress, 5001);
}