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
#include "PauseMenu.h"
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
    void processResumeClick();
    void processMainMenuClick();

signals:
    void mainMenuSignal();

private:
    static const float BUILD_ITEM_SCALE;
    static const float BOARD_ITEM_SCALE;
    static const float MINIMAP_SCALE;

    static const QString FONT_STYLE;
    static const int FONT_SIZE;

    Preferences * preferences;
    QGraphicsScene * scene;
    Cursor * cursor;
    Battlefield * battlefield;

    QGraphicsPixmapItem * minimapBoard;
    Minimap * minimap;
    QGraphicsRectItem * shownArea;

    // TODO: realize as PlayerMenu class
    QGraphicsPixmapItem * playerBoard;
    MenuItem * pauseMenuItem;
    MenuItem * hidePanels;
    MenuItem * buildTowerItem;
    int currentTowerItem;
    int largestTowerWidth;
    QStringList towersTypes;
    MenuItem * scrollForward;
    MenuItem * scrollBackward;
    QGraphicsRectItem * totalBaseHealthBar;
    QGraphicsRectItem * currentBaseHealthBar;
    QGraphicsTextItem * healthInfo;

    PauseMenu * pauseMenu;
    //QGraphicsPixmapItem * pauseMenuBoard; <- inside PauseMenu
    //QList<MenuItem *> listOfItems; <- inside PauseMenu
    bool hide;
};

#endif // GAMEINTERFACE_H
