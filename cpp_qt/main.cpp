#include "scenewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SceneWindow w;
    w.show();

    return a.exec();
}
