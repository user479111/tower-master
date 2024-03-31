#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <QObject>
#include <QStringList>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QRectF>
#include <QList>

#include "Enemy.h"
#include "Tower.h"
#include "Cursor.h"
#include "Minimap.h"
#include "Location.h"
#include "MenuItem.h"
#include "PauseMenu.h"
#include "Battlefield.h"
#include "Preferences.h"
#include "VictoryMenu.h"
#include "GameOverMenu.h"

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

    void displayTowerInfoBoard(const Tower & tower);
    void displayEnemyInfoBoard(const Enemy & enemy);
    void removeInfoBoard();

    void processEscapePress();

private slots:
    void processScroll();
    void processPressEvent();
    void processBattlefieldScale();

    void processEnemyAttack();

    void connectMinimapWithEnemies();

    // Player board slots
    void processPauseClick();
    void processHideClick();
    void processBuildTowerClick();
    void processScrollForward();
    void processScrollBackward();
    void processBuildingTower();

    void processGameOver();
    void processVictory();

    // In game menus' slots
    void processResumeClick();
    void processRestartClick();
    void processMainMenuClick();

    void processTowerClicked(const GameObject * object);

signals:
    void mainMenuSignal();
    void restart();

private:
    static const float BUILD_ITEM_SCALE;
    static const float BOARD_ITEM_SCALE;
    static const float MINIMAP_SCALE;

    static const QString BOARD_FILE;
    static const QString INFO_BOARD_FILE;
    static const float OBJECT_PREVIEW_SCALE;

    static const QString FONT_STYLE;
    static const int FONT_SIZE;
    static const int TITLE_FONT_SIZE;

    Preferences * preferences;
    QGraphicsScene * scene;
    Cursor * cursor;
    Battlefield * battlefield;

    // TODO: move board and shownArea into minimap?
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

    // TODO: separate class GameObjectInfoBoard
    QGraphicsPixmapItem * objectInfoBoard;
    QGraphicsPixmapItem * objectPreview;
    QGraphicsRectItem * totalObjectHealthBar;
    QGraphicsRectItem * currentObjectHealthBar;
    QGraphicsTextItem * objectInfoTitle;
    QGraphicsTextItem * objectInfoText;
    bool infoBoardDisplayed;

    // TODO: seems like the ierarchy is needed here
    // (InGameMenu -> pause, game over, victory, message)
    PauseMenu * pauseMenu;
    GameOverMenu * gameOverMenu;
    VictoryMenu * victoryMenu;
    bool hide;
};

#endif // GAMEINTERFACE_H
