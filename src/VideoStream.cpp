#include "VideoStream.h"
#include <QNetworkDatagram>
#include <QTimer>
#include <QDataStream>
#include <QDateTime>

VideoStream::VideoStream(QObject *parent) : QObject(parent) {
    udpSocket = new QUdpSocket(this);
    connectionTimer = new QTimer(this);
    connectionTimer->setInterval(5000);
    connect(connectionTimer, &QTimer::timeout, [this]() {
        if (QDateTime::currentMSecsSinceEpoch() - lastFrameTime > 5000) {
            emit connectionStatusChanged(false);
        }
    });
}

void VideoStream::startStreaming(quint16 port) {
    if (udpSocket->state() == QUdpSocket::BoundState) {
        udpSocket->close();
    }
    
    if (!udpSocket->bind(port, QUdpSocket::ShareAddress)) {
        qWarning() << "Could not bind to video port" << port;
        return;
    }
    
    connect(udpSocket, &QUdpSocket::readyRead, this, &VideoStream::processPendingDatagrams);
    connectionTimer->start();
    emit connectionStatusChanged(true);
}

void VideoStream::processPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QByteArray data = datagram.data();

        if (data.size() < 8) continue;

        quint32 frame_id;
        quint16 packet_num, total_packets;
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);
        stream >> frame_id >> packet_num >> total_packets;

        QByteArray payload = data.mid(8);

        if (currentFrameId != frame_id) {
            frameBuffer.clear();
            frameBuffer.resize(total_packets);
            receivedPackets = 0;
            currentFrameId = frame_id;
        }

        if (packet_num < total_packets && frameBuffer[packet_num].isEmpty()) {
            frameBuffer[packet_num] = payload;
            receivedPackets++;
        }

        if (receivedPackets == total_packets) {
            QByteArray completeFrame;
            for (const QByteArray &packet : frameBuffer) {
                completeFrame.append(packet);
            }

            QImage image;
            if (image.loadFromData(completeFrame, "JPEG")) {
                lastFrameTime = QDateTime::currentMSecsSinceEpoch();
                emit imageReceived(image);
                emit connectionStatusChanged(true);
            }
        }
    }
}