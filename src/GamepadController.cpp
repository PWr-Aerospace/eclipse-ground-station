#define SDL_MAIN_HANDLED

#include "GamepadController.h"
#include <QHostAddress>
#include <QTimer>

namespace
{
    constexpr int GAMEPAD_DEADZONE = 8000;
    constexpr auto GAMEPAD_POLLING_DELAY_MS = 50;
}

GamepadController::GamepadController(const QString &targetIp,
                                     quint16 targetPort,
                                     QObject *parent)
    : QObject(parent),
      udpSocket(new QUdpSocket(this)),
      ip(targetIp),
      port(targetPort)
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
    {
        emit logMessage(
            QStringLiteral("SDL init failed: %1")
                .arg(QString::fromUtf8(SDL_GetError())));
        return;
    }

    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GamepadController::poll);
    timer->start(GAMEPAD_POLLING_DELAY_MS);
}

GamepadController::~GamepadController()
{
    if (controller)
    {
        emit disconnected();
        SDL_GameControllerClose(controller);
    }
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}

void GamepadController::setControllerIndex(int index)
{
    if (controller)
    {
        SDL_GameControllerClose(controller);
        controller = nullptr;
        emit disconnected();
    }
    if (index >= 0 && index < SDL_NumJoysticks() && SDL_IsGameController(index))
    {
        controller = SDL_GameControllerOpen(index);
        if (controller)
        {
            currentIndex = index;
            prevA = prevB = prevX = prevY = false;
            leftValue = rightValue = 0.0f;
            emit connected();
            emit logMessage(
                QStringLiteral("Gamepad connected: %1")
                    .arg(QString::fromUtf8(SDL_GameControllerName(controller))));
        }
        else
        {
            emit logMessage(
                QStringLiteral("Failed to open gamepad #%1: %2")
                    .arg(index)
                    .arg(QString::fromUtf8(SDL_GetError())));
        }
    }
    else
    {
        emit logMessage(
            QStringLiteral("Invalid gamepad index: %1").arg(index));
    }
}

void GamepadController::poll()
{
    if (!controller)
        return;
    SDL_GameControllerUpdate();

    const Sint16 rawL = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
    const Sint16 rawR = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
    float newL = normalizeAxis(rawL);
    float newR = normalizeAxis(rawR);
    if (newL != leftValue || newR != rightValue)
    {
        leftValue = newL;
        rightValue = newR;
        emit axisChanged(leftValue, rightValue);
    }

    struct Btn
    {
        SDL_GameControllerButton id;
        bool &prev;
    };
    Btn buttons[] = {
        {SDL_CONTROLLER_BUTTON_A, prevA},
        {SDL_CONTROLLER_BUTTON_B, prevB},
        {SDL_CONTROLLER_BUTTON_X, prevX},
        {SDL_CONTROLLER_BUTTON_Y, prevY}};
    for (auto &b : buttons)
    {
        bool pressed = SDL_GameControllerGetButton(controller, b.id);
        if (pressed != b.prev)
        {
            emit buttonChanged(b.id, pressed);
            const char *btnName = SDL_GameControllerGetStringForButton(b.id);
            emit logMessage(
                QStringLiteral("Button %1 %2")
                    .arg(QString::fromUtf8(btnName))
                    .arg(pressed ? "pressed" : "released"));
            b.prev = pressed;
        }
    }

    const QString data = QStringLiteral("Gamepad: L:%1;R:%2")
                             .arg(leftValue, 0, 'f', 2)
                             .arg(rightValue, 0, 'f', 2);
    if (udpSocket)
    {
        udpSocket->writeDatagram(data.toUtf8(), QHostAddress(ip), port);
    }
    emit logMessage(data);
}

float GamepadController::normalizeAxis(Sint16 value)
{
    const int deadZone = 8000;
    if (abs(value) < deadZone)
        return 0.0f;
    return -static_cast<float>(value) / 32767.0f;
}
