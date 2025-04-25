#define SDL_MAIN_HANDLED

#include "GamepadController.h"
#include <QHostAddress>
#include <QTimer>
#include <QDebug>

GamepadController::GamepadController(const QString& targetIp, quint16 targetPort, QObject *parent)
    : QObject(parent),
      udpSocket(new QUdpSocket(this)),
      ip(targetIp),
      port(targetPort)
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0) {
        return;
    }

    int totalJoysticks = SDL_NumJoysticks();

    bool found = false;
    for (int i = 0; i < totalJoysticks; ++i) {
        if (SDL_IsGameController(i)) {
            if (!controller) {
                controller = SDL_GameControllerOpen(i);
                if (controller) {
                    found = true;
                } else {
                }
            }
        }
    }

    if (!found) {
        return;
    }

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GamepadController::poll);
    timer->start(50);
}

GamepadController::~GamepadController() {
    if (controller) {
        SDL_GameControllerClose(controller);
    }
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}

void GamepadController::poll() {
    if (!controller) {
        return;
    }

    SDL_GameControllerUpdate();

    Sint16 leftY_raw = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
    Sint16 rightY_raw = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

    leftValue = normalizeAxis(leftY_raw);
    rightValue = normalizeAxis(rightY_raw);

    QString data = QString("Gamepad: L:%1;R:%2")
                .arg(leftValue, 0, 'f', 2)
                .arg(rightValue, 0, 'f', 2);


    udpSocket->writeDatagram(data.toUtf8(), QHostAddress(ip), port);

    emit logMessage(data);  
}

float GamepadController::normalizeAxis(Sint16 value) {
    const int deadZone = 8000;
    if (abs(value) < deadZone) return 0.0f;
    return -static_cast<float>(value) / 32767.0f;
}
