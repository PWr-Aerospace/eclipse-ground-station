#include "VideoReceiver.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    VideoReceiver receiver;
    receiver.show();
    
    return app.exec();
}