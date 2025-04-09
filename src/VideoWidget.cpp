#include "VideoWidget.h"
#include "VideoStream.h"

#include <QVBoxLayout>
#include <QLabel>

VideoWidget::VideoWidget(QWidget *parent) : QWidget(parent) {
    videoStream = new VideoStream(this);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setText("Waiting for video stream...");
    imageLabel->setStyleSheet("QLabel { background-color: black; color: white; }");
    
    statusLabel = new QLabel("Disconnected", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    
    layout->addWidget(imageLabel, 1);
    layout->addWidget(statusLabel);
    
    connect(videoStream, &VideoStream::imageReceived, 
            this, &VideoWidget::onImageReceived);
    connect(videoStream, &VideoStream::connectionStatusChanged,
            this, &VideoWidget::connectionStatusChanged);
}

VideoWidget::~VideoWidget() {
    delete videoStream;
}

void VideoWidget::startStreaming(quint16 port) {
    videoStream->startStreaming(port);
    statusLabel->setText("Connecting...");
}

void VideoWidget::stopStreaming() {
    statusLabel->setText("Disconnected");
}

void VideoWidget::onImageReceived(const QImage &image) {
    imageLabel->setPixmap(QPixmap::fromImage(image).scaled(
        imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    statusLabel->setText("Streaming active");
}