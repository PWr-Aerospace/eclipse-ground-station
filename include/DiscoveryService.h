#ifndef DISCOVERYSERVICE_H
#define DISCOVERYSERVICE_H

#include <QObject>
#include <QUdpSocket>

class DiscoveryService : public QObject {
    Q_OBJECT
public:
    explicit DiscoveryService(QObject *parent = nullptr);

signals:
    void deviceDiscovered(const QString &address, quint16 port);

public slots:
    void startDiscovery(const QHostAddress &senderAddress);

private slots:
    void processDiscoveryDatagrams();

private:
    QUdpSocket *discoverySocket;
};

#endif // DISCOVERYSERVICE_H