#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QImage>

class QLabel;
class VideoStream;

class VideoWidget : public QWidget {
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);
    ~VideoWidget();

    void startStreaming(quint16 port);
    void stopStreaming();

signals:
    void connectionStatusChanged(bool connected);

private slots:
    void onImageReceived(const QImage &image);

private:
    VideoStream *videoStream;
    QLabel *imageLabel;
    QLabel *statusLabel;
};

#endif // VIDEOWIDGET_H