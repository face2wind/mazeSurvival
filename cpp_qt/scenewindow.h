#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include <QMainWindow>

namespace Ui {
class SceneWindow;
}

class SceneWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SceneWindow(QWidget *parent = 0);
    ~SceneWindow();

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::SceneWindow *ui;

    int MAP_WIDTH = 1000;
    int MAP_HEIGHT = 600;
};

#endif // SCENEWINDOW_H
