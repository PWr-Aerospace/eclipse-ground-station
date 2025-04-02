#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <QObject>
#include <QUdpSocket>
#include <QImage>

class VideoStream : public QObject {
    Q_OBJECT
public:
    explicit VideoStream(QObject *parent = nullptr);
    void startStreaming(quint16 port);

signals:
    void imageReceived(const QImage &image);
    void connectionStatusChanged(bool connected);

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *udpSocket;
    quint32 currentFrameId = 0;
    QVector<QByteArray> frameBuffer;
    int receivedPackets = 0;
    qint64 lastFrameTime = 0;
    QTimer *connectionTimer;
};

#endif // VIDEOSTREAM_H