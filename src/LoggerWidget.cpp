#include "LoggerWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

LoggerWidget::LoggerWidget(QWidget *parent)
    : QWidget(parent),
      textEdit(new QTextEdit(this)),
      filterBox(new QComboBox(this))
{
    textEdit->setReadOnly(true);

    filterBox->addItem("Wszystko");
    filterBox->addItem("Gamepad");
    filterBox->addItem("UDP");
    connect(filterBox, &QComboBox::currentTextChanged,
            this, &LoggerWidget::filterChanged);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(filterBox);
    layout->addLayout(topLayout);
    layout->addWidget(textEdit);
}

void LoggerWidget::appendLog(const QString &text) {
    if (currentFilter == "Wszystko" || text.contains(currentFilter, Qt::CaseInsensitive)) {
        textEdit->append(text);
    }
}

void LoggerWidget::filterChanged(const QString &filter) {
    currentFilter = filter;
    textEdit->clear();
}
