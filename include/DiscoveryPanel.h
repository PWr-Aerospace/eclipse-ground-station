#ifndef DISCOVERYPANEL_H
#define DISCOVERYPANEL_H

#include <QWidget>
#include <QMap>

class QComboBox;
class QPushButton;
class QLabel;
class DiscoveryService;

class DiscoveryPanel : public QWidget {
    Q_OBJECT
public:
    explicit DiscoveryPanel(QWidget *parent = nullptr);
    ~DiscoveryPanel();

    QString currentDeviceAddress() const;
    quint16 currentDevicePort() const;

public slots:
    void updateConnectionStatus(bool connected);

signals:
    void connectClicked();
    void refreshClicked();

private slots:
    void onDeviceDiscovered(const QString &address, quint16 port);

private:
    void setupUI();

    DiscoveryService *discoveryService;
    QComboBox *devicesCombo;
    QPushButton *connectButton;
    QPushButton *refreshButton;
    QLabel *statusLabel;
    
    QMap<QString, quint16> discoveredDevices;
    bool isConnected = false;
};

#endif // DISCOVERYPANEL_H