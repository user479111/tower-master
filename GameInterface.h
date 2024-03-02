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
#include "Battlefield.h"
#include "MenuItem.h"

class GameInterface : public QObject
{
    Q_OBJECT
public:
    explicit GameInterface(QSharedPointer<QGraphicsScene> scene,
                           QSharedPointer<Cursor> cursor,
                           QSharedPointer<Battlefield> battlefield,
                           QSharedPointer<QObject> parent = nullptr);
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

signals:
    void mainMenuSignal();

private:
    QSharedPointer<QGraphicsScene> scene;
    QSharedPointer<Cursor> cursor;
    QSharedPointer<Battlefield> battlefield;

    QSharedPointer<QGraphicsPixmapItem> minimapBoard;
    QSharedPointer<Minimap> minimap;
    QSharedPointer<QGraphicsRectItem> shownArea;
    QList<QSharedPointer<QGraphicsRectItem>> towers;
    QList<QSharedPointer<QGraphicsRectItem>> enemies;

    // TODO: realize as PlayerMenu class
    QSharedPointer<QGraphicsPixmapItem> playerBoard;
    QSharedPointer<MenuItem> pauseMenu;
    QSharedPointer<MenuItem> hidePanels;
    QSharedPointer<MenuItem> buildTowerItem;
    int currentTowerItem;
    QStringList towersTypes;
    QSharedPointer<MenuItem> scrollForward;
    QSharedPointer<MenuItem> scrollBackward;

    //PauseMenu * pauseMenu;
    //QGraphicsPixmapItem * pauseMenuBoard; <- inside PauseMenu
    //QList<MenuItem *> listOfItems; <- inside PauseMenu
    bool hide;
};

#endif // GAMEINTERFACE_H
