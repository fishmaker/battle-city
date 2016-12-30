#include "game/gameengine.h"
#include <QApplication>
#include <exception>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameEngine w;
    try {
        w.Prepare();
        w.AddLabels();
    } catch (std::exception& e) {
        qDebug() << e.what();
    }
    w.show();

    return a.exec();
}
