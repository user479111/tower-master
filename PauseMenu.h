#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QObject>

#include "Menu.h"
#include "Preferences.h"

class PauseMenu : public Menu, public QGraphicsRectItem
{
    Q_OBJECT
public:
    PauseMenu(const Preferences * preferences);
    ~PauseMenu();

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);

public slots:
    void processResumeClick();
    void processMainMenuClick();

signals:
    void resumeClicked();
    void mainMenuClicked();

private:
    static const QString XML_FILE_NAME;
};

#endif // PAUSEMENU_H
