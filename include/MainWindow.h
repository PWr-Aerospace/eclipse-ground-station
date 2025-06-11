#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTabWidget>
#include <QTimer>

class DiscoveryPanel;
class VideoWidget;
class LoggerWidget;
class GamepadController;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupConnections();

    // top-panel widgets
    QLabel       *chooseGamepadLabel;
    QComboBox    *gamepadComboBox;
    QPushButton  *connectGamepadButton;
    QTimer       *gamepadListTimer;
    QLabel       *chooseRobotLabel;
    DiscoveryPanel *discoveryPanel;

    // main tabs
    QTabWidget   *tabWidget;
    VideoWidget  *videoWidget;
    LoggerWidget *logger;

    // gamepad status widgets
    GamepadController *gamepadController;
    QLabel            *gamepadStatusLabel;
    QLabel            *leftThumbLabel;
    QLabel            *rightThumbLabel;
    QLabel            *buttonALabel;
    QLabel            *buttonBLabel;
    QLabel            *buttonXLabel;
    QLabel            *buttonYLabel;

private slots:
    // slots to update those labels
    void onGamepadConnected();
    void onGamepadDisconnected();
    void onAxisChanged(float left, float right);
    void onButtonChanged(int button, bool pressed);

    // utility slots
    void appendLog(const QString &text);
    void refreshGamepadList();
};
