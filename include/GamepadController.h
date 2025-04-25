#pragma once

#include <QObject>
#include <QUdpSocket>
#include <SDL.h>
#include <SDL_gamecontroller.h>
#define SDL_MAIN_HANDLED 

class GamepadController : public QObject {
    Q_OBJECT

public:
    explicit GamepadController(const QString& targetIp, quint16 targetPort, QObject *parent = nullptr);
    ~GamepadController();

private slots:
    void poll();

private:
    SDL_GameController *controller = nullptr;
    QUdpSocket *udpSocket;

    QString ip;
    quint16 port;

    float leftValue = 0.0f;
    float rightValue = 0.0f;

    float normalizeAxis(Sint16 value);
signals:
    void logMessage(const QString &text);

};
