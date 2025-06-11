#pragma once

#define SDL_MAIN_HANDLED
#include <QObject>
#include <QString>
#include <QUdpSocket>
#include <SDL.h>
#include <SDL_gamecontroller.h>

class GamepadController : public QObject
{
    Q_OBJECT

public:
    explicit GamepadController(const QString &targetIp,
                               quint16 targetPort,
                               QObject *parent = nullptr);
    ~GamepadController();

signals:
    void connected();
    void disconnected();
    void axisChanged(float left, float right);
    void buttonChanged(int button, bool pressed);
    void logMessage(const QString &text);

public slots:
    void poll();
    void setControllerIndex(int index);

private:
    SDL_GameController *controller = nullptr;
    QUdpSocket *udpSocket = nullptr;

    QString ip;
    quint16 port;

    float leftValue = 0.0f;
    float rightValue = 0.0f;

    bool prevA = false;
    bool prevB = false;
    bool prevX = false;
    bool prevY = false;

    int currentIndex = -1;

    float normalizeAxis(Sint16 value);
};
