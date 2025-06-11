#include "MainWindow.h"
#include "DiscoveryPanel.h"
#include "VideoWidget.h"
#include "LoggerWidget.h"
#include "GamepadController.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QTimer>
#include <QDebug>
#include <SDL.h>
#include <SDL_gamecontroller.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      discoveryPanel(new DiscoveryPanel(this)),
      videoWidget(new VideoWidget(this)),
      tabWidget(new QTabWidget(this)),
      logger(new LoggerWidget(this))
{
    setupUI();
    setupConnections();
    resize(1000, 700);
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);

    auto *topPanel = new QWidget(central);
    auto *topLayout = new QHBoxLayout(topPanel);
    topLayout->setContentsMargins(0, 0, 0, 10);

    chooseGamepadLabel = new QLabel(tr("Choose Gamepad"), topPanel);
    gamepadComboBox = new QComboBox(topPanel);
    connectGamepadButton = new QPushButton(tr("Connect"), topPanel);

    gamepadListTimer = new QTimer(this);
    gamepadListTimer->setInterval(5000);
    connect(gamepadListTimer, &QTimer::timeout,
            this, &MainWindow::refreshGamepadList);
    gamepadListTimer->start();
    refreshGamepadList();

    if (gamepadComboBox->count() > 0)
    {
        gamepadComboBox->setCurrentIndex(0);
    }

    auto *gpControlLayout = new QHBoxLayout;
    gpControlLayout->addWidget(gamepadComboBox);
    gpControlLayout->addWidget(connectGamepadButton);

    auto *gpLayout = new QVBoxLayout;
    gpLayout->addWidget(chooseGamepadLabel);
    gpLayout->addLayout(gpControlLayout);

    auto *gpWidget = new QWidget(topPanel);
    gpWidget->setLayout(gpLayout);
    topLayout->addWidget(gpWidget);

    chooseRobotLabel = new QLabel(tr("Choose Robot"), topPanel);
    auto *rbLayout = new QVBoxLayout;
    rbLayout->addWidget(chooseRobotLabel);
    rbLayout->addWidget(discoveryPanel);
    auto *rbWidget = new QWidget(topPanel);
    rbWidget->setLayout(rbLayout);
    topLayout->addWidget(rbWidget);

    topLayout->addStretch();
    mainLayout->addWidget(topPanel);

    gamepadController = new GamepadController("xxxx", 0000, this);

    auto *videoTab = new QWidget(central);
    auto *videoLayout = new QVBoxLayout(videoTab);
    videoLayout->addWidget(videoWidget);

    auto *statusLayout = new QHBoxLayout;
    gamepadStatusLabel = new QLabel(tr("Gamepad: Disconnected"), videoTab);
    leftThumbLabel = new QLabel(tr("LeftY: 0.00"), videoTab);
    rightThumbLabel = new QLabel(tr("RightY: 0.00"), videoTab);
    buttonALabel = new QLabel(tr("A: Up"), videoTab);
    buttonBLabel = new QLabel(tr("B: Up"), videoTab);
    buttonXLabel = new QLabel(tr("X: Up"), videoTab);
    buttonYLabel = new QLabel(tr("Y: Up"), videoTab);

    statusLayout->addWidget(gamepadStatusLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(leftThumbLabel);
    statusLayout->addWidget(rightThumbLabel);
    statusLayout->addWidget(buttonALabel);
    statusLayout->addWidget(buttonBLabel);
    statusLayout->addWidget(buttonXLabel);
    statusLayout->addWidget(buttonYLabel);
    videoLayout->addLayout(statusLayout);

    tabWidget->addTab(videoTab, tr("Video Stream"));
    tabWidget->addTab(new QWidget(central), tr("Controls"));
    tabWidget->addTab(logger, tr("Logs"));

    mainLayout->addWidget(tabWidget, 1);
    setCentralWidget(central);
}

void MainWindow::setupConnections()
{
    connect(discoveryPanel, &DiscoveryPanel::connectClicked, this, [this]()
            { videoWidget->startStreaming(discoveryPanel->currentDevicePort()); });
    connect(videoWidget, &VideoWidget::connectionStatusChanged,
            discoveryPanel, &DiscoveryPanel::updateConnectionStatus);

    connect(gamepadController, &GamepadController::logMessage,
            this, &MainWindow::appendLog);
    connect(gamepadController, &GamepadController::logMessage,
            logger, &LoggerWidget::appendLog);

    connect(gamepadComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int idx)
            { appendLog(tr("Selected gamepad: %1")
                            .arg(gamepadComboBox->currentText())); });
    connect(connectGamepadButton, &QPushButton::clicked, this, [this]()
            {
        int idx = gamepadComboBox->currentData().toInt();
        gamepadController->setControllerIndex(idx); });

    connect(gamepadController, &GamepadController::connected,
            this, &MainWindow::onGamepadConnected);
    connect(gamepadController, &GamepadController::disconnected,
            this, &MainWindow::onGamepadDisconnected);
    connect(gamepadController, &GamepadController::axisChanged,
            this, &MainWindow::onAxisChanged);
    connect(gamepadController, &GamepadController::buttonChanged,
            this, &MainWindow::onButtonChanged);
}

void MainWindow::refreshGamepadList()
{
    gamepadComboBox->blockSignals(true);
    gamepadComboBox->clear();

    if (!(SDL_WasInit(SDL_INIT_GAMECONTROLLER) & SDL_INIT_GAMECONTROLLER))
    {
        SDL_Init(SDL_INIT_GAMECONTROLLER);
    }

    int count = SDL_NumJoysticks();
    for (int i = 0; i < count; ++i)
    {
        if (SDL_IsGameController(i))
        {
            QString name = QString::fromUtf8(SDL_GameControllerNameForIndex(i));
            gamepadComboBox->addItem(name, i);
        }
    }

    gamepadComboBox->blockSignals(false);
}

void MainWindow::appendLog(const QString &text)
{
    if (logger)
        logger->appendLog(text);
}

void MainWindow::onGamepadConnected()
{
    qDebug() << "Slot onGamepadConnected()";
    gamepadStatusLabel->setText(tr("Gamepad: Connected"));
}

void MainWindow::onGamepadDisconnected()
{
    qDebug() << "Slot onGamepadDisconnected()";
    gamepadStatusLabel->setText(tr("Gamepad: Disconnected"));
}

void MainWindow::onAxisChanged(float left, float right)
{
    qDebug() << "Slot onAxisChanged(): left=" << left << " right=" << right;
    leftThumbLabel->setText(tr("LeftY: %1").arg(left, 0, 'f', 2));
    rightThumbLabel->setText(tr("RightY: %1").arg(right, 0, 'f', 2));
}

void MainWindow::onButtonChanged(int btn, bool pressed)
{
    qDebug() << "Slot onButtonChanged(): btn=" << btn << " pressed=" << pressed;
    const QString state = pressed ? tr("Down") : tr("Up");
    switch (btn)
    {
    case SDL_CONTROLLER_BUTTON_A:
        buttonALabel->setText(tr("A: %1").arg(state));
        break;
    case SDL_CONTROLLER_BUTTON_B:
        buttonBLabel->setText(tr("B: %1").arg(state));
        break;
    case SDL_CONTROLLER_BUTTON_X:
        buttonXLabel->setText(tr("X: %1").arg(state));
        break;
    case SDL_CONTROLLER_BUTTON_Y:
        buttonYLabel->setText(tr("Y: %1").arg(state));
        break;
    }
}
