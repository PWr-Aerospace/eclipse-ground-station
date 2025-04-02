#ifndef VIDEORECEIVER_H
#define VIDEORECEIVER_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>

class DiscoveryService;
class VideoStream;

class VideoReceiver : public QMainWindow {
    Q_OBJECT
public:
    explicit VideoReceiver(QWidget *parent = nullptr);
    ~VideoReceiver();

private slots:
    void onDeviceDiscovered(const QString &address, quint16 port);
    void onImageReceived(const QImage &image);
    void onConnectionStatusChanged(bool connected);
    void connectToSelectedDevice();
    void clearDevicesList();

private:
    void setupUI();

    QLabel *imageLabel;
    QLabel *statusLabel;
    QComboBox *devicesCombo;
    QPushButton *connectButton;
    QPushButton *refreshButton;
    
    DiscoveryService *discoveryService;
    VideoStream *videoStream;
    
    QMap<QString, quint16> discoveredDevices;
};

#endif // VIDEORECEIVER_H