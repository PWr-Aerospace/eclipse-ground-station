#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>

class LoggerWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoggerWidget(QWidget *parent = nullptr);

public slots:
    void appendLog(const QString &text);

private slots:
    void filterChanged(const QString &filter);

private:
    QTextEdit *textEdit;
    QComboBox *filterBox;
    QString currentFilter;
};
