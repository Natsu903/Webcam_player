#include "webcam_player.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Webcam_player w;
    w.show();
    return a.exec();
}
