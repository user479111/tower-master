#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <QObject>
#include <QStringList>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QRectF>
#include <QList>

#include "Cursor.h"
#include "Minimap.h"
#include "Location.h"
#include "MenuItem.h"
#include "Battlefield.h"
#include "Preferences.h"

class GameInterface : public QObject
{
    Q_OBJECT
public:
    explicit GameInterface(Preferences * preferences,
                           QGraphicsScene * scene,
                           Cursor * cursor,
                           Battlefield * battlefield,
                           QObject * parent = nullptr);
    ~GameInterface();

private slots:
    void processScroll();
    void processPressEvent();
    void processBattlefieldScale();
    void processPauseClick();
    void processHideClick();
    void processBuildTowerClick();
    void processScrollForward();
    void processScrollBackward();
    void processBuildingTower();
    void connectMinimapWithEnemies();

signals:
    void mainMenuSignal();

private:
    Preferences * preferences;
    QGraphicsScene * scene;
    Cursor * cursor;
    Battlefield * battlefield;

    QGraphicsPixmapItem * minimapBoard;
    Minimap * minimap;
    QGraphicsRectItem * shownArea;

    // TODO: realize as PlayerMenu class
    QGraphicsPixmapItem * playerBoard;
    MenuItem * pauseMenu;
    MenuItem * hidePanels;
    MenuItem * buildTowerItem;
    int currentTowerItem;
    QStringList towersTypes;
    MenuItem * scrollForward;
    MenuItem * scrollBackward;

    //PauseMenu * pauseMenu;
    //QGraphicsPixmapItem * pauseMenuBoard; <- inside PauseMenu
    //QList<MenuItem *> listOfItems; <- inside PauseMenu
    bool hide;
};

#endif // GAMEINTERFACE_H
