#include "GameInterface.h"
#include "Tower.h"

#include <QDir>
#include <QDebug>

GameInterface::GameInterface(Preferences * preferences,
                             QGraphicsScene * scene,
                             Cursor * cursor,
                             Battlefield * battlefield,
                             QObject * parent) :
    QObject(parent),
    preferences(preferences),
    scene(scene),
    cursor(cursor),
    battlefield(battlefield),
    pauseMenuItem(new MenuItem()),
    hidePanels(new MenuItem()),
    buildTowerItem(new MenuItem()),
    currentTowerItem(0),
    scrollForward(new MenuItem()),
    scrollBackward(new MenuItem()),
    pauseMenu(new PauseMenu(preferences)),
    hide(false)
{
    cursor->setScrollAreaRect(battlefield->getLocation()->mapRectToScene(battlefield->getLocation()->boundingRect()));

    connect(cursor, &Cursor::areaScrolled, this, &GameInterface::processScroll);

    // show minimap board
    minimapBoard = new QGraphicsPixmapItem(QPixmap(":/Data/Data/Game/MinimapDesk.png")); // all the paths should be written somewhere
    minimapBoard->setX(scene->sceneRect().right() - minimapBoard->boundingRect().width());
    minimapBoard->setY(scene->sceneRect().bottom() - minimapBoard->boundingRect().height());
    minimapBoard->setZValue(1);
    scene->addItem(minimapBoard);

    // show minimap
    minimap = new Minimap(QPixmap(battlefield->getLocation()->getLocationImagePath()));
    minimap->setScale(0.1); // TODO: xml
    minimap->setX(minimapBoard->x() +
                    (minimapBoard->boundingRect().width() - minimap->boundingRect().width() * minimap->scale()) / 2);
    minimap->setY(minimapBoard->y() +
                    (minimapBoard->boundingRect().height() - minimap->boundingRect().height() * minimap->scale()) / 2);
    minimap->setZValue(1);
    scene->addItem(minimap);

    // process press on minimap
    connect(minimap, &Minimap::mousePressed, this, &GameInterface::processPressEvent);

    // draw shown area on minimap
    shownArea = new QGraphicsRectItem(0, 0, scene->sceneRect().width() * 0.1,
                                            scene->sceneRect().height() * 0.1);
    shownArea->setPos(minimap->x() + scene->sceneRect().x()* 0.1,
                      minimap->y() + scene->sceneRect().y()* 0.1);
    shownArea->setZValue(1);
    shownArea->setBrush(Qt::transparent); // Set the fill color
    shownArea->setPen(QPen(Qt::red));
    scene->addItem(shownArea);

    // process map scale
    connect(battlefield, &Battlefield::battlefieldScaled, this, &GameInterface::processBattlefieldScale);

    // show player board
    playerBoard = new QGraphicsPixmapItem(QPixmap(":/Data/Data/Game/MinimapDesk.png")); // all the paths should be written somewhere
    playerBoard->setX(scene->sceneRect().left());
    playerBoard->setY(scene->sceneRect().bottom() - playerBoard->boundingRect().height());
    playerBoard->setZValue(1);
    scene->addItem(playerBoard);

    // setup pause menu button
    pauseMenuItem->setPixmap(QString(":/Data/Data/Menu/Scroll.png"));

    // move to XML?
    if (preferences->getLanguage() == "English") {
        pauseMenuItem->setText("Pause");
    } else if (preferences->getLanguage() == "Українська") {
        pauseMenuItem->setText("Пауза");
    } else if (preferences->getLanguage() == "Русский") {
        pauseMenuItem->setText("Пауза");
    }

    pauseMenuItem->setScale(0.5 /* xml */);
    pauseMenuItem->setPos(QPointF(
        playerBoard->x() + (playerBoard->boundingRect().width() -
                               pauseMenuItem->boundingRect().width() * 2 *
                               pauseMenuItem->scale()) / 3,
        playerBoard->y() + pauseMenuItem->boundingRect().height() *
                              pauseMenuItem->scale() / 2));
    pauseMenuItem->setZValue(1);
    pauseMenuItem->show(scene);

    connect(pauseMenuItem, &MenuItem::clicked, this, &GameInterface::processPauseClick);

    // setup hide menu button
    hidePanels->setPixmap(QString(":/Data/Data/Menu/Scroll.png"));

    // TODO: move to XML?
    if (preferences->getLanguage() == "English") {
        hidePanels->setText("Hide");
    } else if (preferences->getLanguage() == "Українська") {
        hidePanels->setText("Сховати");
    } else if (preferences->getLanguage() == "Русский") {
        hidePanels->setText("Спрятать");
    }

    hidePanels->setScale(0.5 /* xml */ );
    hidePanels->setPos(QPointF(
        playerBoard->x() + playerBoard->boundingRect().width() - hidePanels->boundingRect().width() * hidePanels->scale() -
                           (playerBoard->boundingRect().width() - hidePanels->boundingRect().width() * hidePanels->scale() * 2) / 3,
        playerBoard->y() + hidePanels->boundingRect().height() * hidePanels->scale() / 2));
    hidePanels->setZValue(1);
    hidePanels->show(scene);

    connect(hidePanels, &MenuItem::clicked, this, &GameInterface::processHideClick);

    // setup build tower icon
    towersTypes = QDir(":/Data/Data/Towers/").entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    buildTowerItem->setTitle("build-tower");
    buildTowerItem->setPixmap(QString(":/Data/Data/Towers/" + towersTypes[currentTowerItem] + "/Tower.png"));
    buildTowerItem->setPos(QPointF(
        playerBoard->x() + playerBoard->boundingRect().width() / 2 - buildTowerItem->boundingRect().width() / 2,
        playerBoard->y() + playerBoard->boundingRect().height() / 2 - buildTowerItem->boundingRect().height() / 2));
    buildTowerItem->setZValue(1);
    buildTowerItem->setStaticSize(true);
    scene->addItem(buildTowerItem);

    connect(buildTowerItem, &MenuItem::clicked, this, &GameInterface::processBuildTowerClick);

    // Setup ScrollForward button
    scrollForward->setTitle("scroll-forward");
    scrollForward->setPixmap(QString(":/Data/Data/Game/ArrowLeft.png"));
    scrollForward->setPos(QPointF(
        buildTowerItem->x() - scrollForward->boundingRect().width() * 1.1,
        playerBoard->y() + playerBoard->boundingRect().height() / 2 - scrollForward->boundingRect().height() / 2));
    scrollForward->setZValue(1);
    scrollForward->setStaticSize(true);
    scene->addItem(scrollForward);

    connect(scrollForward, &MenuItem::clicked, this, &GameInterface::processScrollForward);

    // setup ScrollBackward button
    scrollBackward->setTitle("scroll-backward");
    scrollBackward->setPixmap(QString(":/Data/Data/Game/ArrowRight.png"));
    scrollBackward->setPos(QPointF(
        buildTowerItem->x() + buildTowerItem->boundingRect().width() + scrollBackward->boundingRect().width() * 0.1,
        playerBoard->y() + playerBoard->boundingRect().height() / 2 - scrollBackward->boundingRect().height() / 2));
    scrollBackward->setStaticSize(true);
    scrollBackward->setZValue(1);
    scene->addItem(scrollBackward);

    connect(scrollBackward, &MenuItem::clicked, this, &GameInterface::processScrollBackward);

    connect(battlefield, &Battlefield::enemiesHaveBeenRun, this, &GameInterface::connectMinimapWithEnemies);

    connect(pauseMenu, &PauseMenu::resumeClicked, this, &GameInterface::processResumeClick);
    connect(pauseMenu, &PauseMenu::mainMenuClicked, this, &GameInterface::processMainMenuClick);
}

GameInterface::~GameInterface()
{
    delete minimapBoard;

    delete minimap;

    delete shownArea;

    delete playerBoard;

    delete pauseMenuItem;

    delete hidePanels;

    delete buildTowerItem;

    delete scrollForward;

    delete scrollBackward;

    delete pauseMenu;

    auto sceneRect = scene->sceneRect();
    sceneRect.translate(-scene->sceneRect().left(), -scene->sceneRect().top());
    scene->setSceneRect(sceneRect);

    // Update corresponding cursor parameters
    cursor->setScrollAreaRect(scene->sceneRect());
}

// Move all the interface items with the scene
void GameInterface::processScroll()
{
    minimapBoard->setX(scene->sceneRect().right() - minimapBoard->boundingRect().width());
    minimapBoard->setY(scene->sceneRect().bottom() - minimapBoard->boundingRect().height() +
                       hide * (playerBoard->boundingRect().height() / 2));

    minimap->setPos(QPointF(
                       minimapBoard->x() + (minimapBoard->boundingRect().width() -
                                            minimap->boundingRect().width() * minimap->scale()) / 2,
                       minimapBoard->y() + (minimapBoard->boundingRect().height() -
                                            minimap->boundingRect().height() * minimap->scale()) / 2));

    auto scenePosOnMap = battlefield->getLocation()->mapFromScene(scene->sceneRect().topLeft());
    shownArea->setPos(minimap->pos() + scenePosOnMap * 0.1);

    playerBoard->setX(scene->sceneRect().left());
    playerBoard->setY(scene->sceneRect().bottom() - playerBoard->boundingRect().height() +
                      hide * (playerBoard->boundingRect().height() / 2));

    pauseMenuItem->setPos(QPointF(
        playerBoard->x() + (playerBoard->boundingRect().width() -
                            pauseMenuItem->boundingRect().width() *
                            pauseMenuItem->scale() * 2) / 3,
        playerBoard->y() + pauseMenuItem->boundingRect().height() *
                          pauseMenuItem->scale() / 2));

    hidePanels->setPos(QPointF(
        playerBoard->x() + playerBoard->boundingRect().width() -
                           hidePanels->boundingRect().width() *
                           hidePanels->scale() -
                           (playerBoard->boundingRect().width() -
                            hidePanels->boundingRect().width() *
                            hidePanels->scale() * 2) / 3,
        playerBoard->y() + hidePanels->boundingRect().height() *
                           hidePanels->scale() / 2));

    buildTowerItem->setPos(QPointF(
        playerBoard->x() + playerBoard->boundingRect().width() / 2 - buildTowerItem->boundingRect().width() / 2,
        playerBoard->y() + playerBoard->boundingRect().height() / 2 - buildTowerItem->boundingRect().height() / 2));

    scrollForward->setPos(QPointF(
        buildTowerItem->x() - scrollForward->boundingRect().width() * 1.1,
        playerBoard->y() + playerBoard->boundingRect().height() / 2 - scrollForward->boundingRect().height() / 2));

    scrollBackward->setPos(QPointF(
        buildTowerItem->x() + buildTowerItem->boundingRect().width() + scrollBackward->boundingRect().width() * 0.1,
        playerBoard->y() + playerBoard->boundingRect().height() / 2 - scrollBackward->boundingRect().height() / 2));
}

void GameInterface::processPressEvent()
{
    if (cursor->getBuildMode()) {
        return;
    }

    auto sceneRect = scene->sceneRect();
    auto locationRect = battlefield->getLocation()->mapRectToScene(battlefield->getLocation()->boundingRect());

    // Move sceneRect to the point on the map that represents the click point on the minimap
    // Take into account that the location could be moved during scale procedure
    sceneRect.moveCenter(QPointF(locationRect.left() + minimap->getCalculatedScenePosition().x(),
                            locationRect.top() + minimap->getCalculatedScenePosition().y()));
    scene->setSceneRect(sceneRect);

    processScroll();

    // Cursor should stay on the same place for the player
    cursor->setPos(QPointF(minimap->x() + minimap->getClickPosition().x(),
                           minimap->y() + minimap->getClickPosition().y()));
}

void GameInterface::processBattlefieldScale()
{
    // Update corresponding minimap parameters
    minimap->setMapScale(battlefield->getLocation()->scale());


    // Redraw shownArea on minimap
    auto scenePosOnMap = battlefield->getLocation()->mapFromScene(scene->sceneRect().topLeft());
    shownArea->setPos(minimap->pos() + scenePosOnMap * 0.1 /* move scale to xml */);
    shownArea->setScale(
                (minimap->boundingRect().width() * 0.1 /* move scale to xml */ * scene->sceneRect().width() /
                 (battlefield->getLocation()->boundingRect().width() * battlefield->getLocation()->scale())) /
                shownArea->boundingRect().width());

    processScroll();
}

void GameInterface::processPauseClick()
{
    // Pause battlefield events
    battlefield->pause();

    // Pause map scroll
    cursor->setScrollAreaRect(scene->sceneRect());

    // Show the Pause menu
    pauseMenu->show(scene);
}

void GameInterface::processHideClick()
{
    hide = !hide;

    if (preferences->getLanguage() == "English") {
        hidePanels->setText(hide ? "Show" : "Hide");
    } else if (preferences->getLanguage() == "Українська") {
        hidePanels->setText(hide ? "Показати" : "Сховати");
    } else if (preferences->getLanguage() == "Русский") {
        hidePanels->setText(hide ? "Показать" : "Спрятать");
    }

    processScroll();
}

void GameInterface::processBuildTowerClick()
{
    cursor->setBuildMode(true, towersTypes[currentTowerItem], battlefield->getLocation()->scale());

    connect(cursor, &Cursor::mousePressed, this, &GameInterface::processBuildingTower);
}

void GameInterface::processScrollForward()
{
    currentTowerItem = (currentTowerItem + 1) % towersTypes.size(); // increment until towersTypes.size()

    buildTowerItem->setPixmap(QString(":/Data/Data/Towers/" + towersTypes[currentTowerItem] + "/Tower.png"));

    // Set the Icon on the updated position
    buildTowerItem->setPos(QPointF(
        playerBoard->x() + playerBoard->boundingRect().width() / 2 - buildTowerItem->boundingRect().width() / 2,
        playerBoard->y() + playerBoard->boundingRect().height() / 2 - buildTowerItem->boundingRect().height() / 2));
}

void GameInterface::processScrollBackward()
{
    currentTowerItem = (currentTowerItem > 0 ? currentTowerItem : towersTypes.size()) - 1; // decrement until 0

    buildTowerItem->setPixmap(QString(":/Data/Data/Towers/" + towersTypes[currentTowerItem] + "/Tower.png"));

    // Set the Icon on the updated position
    buildTowerItem->setPos(QPointF(
        playerBoard->x() + playerBoard->boundingRect().width() / 2 - buildTowerItem->boundingRect().width() / 2,
        playerBoard->y() + playerBoard->boundingRect().height() / 2 - buildTowerItem->boundingRect().height() / 2));
}

void GameInterface::processBuildingTower()
{
    if (!cursor->getBuildMode() || !cursor->getBuildIsPossible()) {
        return;
    }

    cursor->setBuildMode(false);

    Tower * tower = new Tower(towersTypes[currentTowerItem], cursor->pos(), battlefield->getLocation());

    // Display the tower on the battlefield
    battlefield->addTower(tower);

    // Display the tower position on the minimap
    minimap->addTower(tower);

    disconnect(cursor, &Cursor::mousePressed, this, &GameInterface::processBuildingTower);
}

void GameInterface::connectMinimapWithEnemies()
{
    minimap->connectWithEenemies(battlefield->getGroupOfEnemies());
}

void GameInterface::processResumeClick()
{
    // Hide the Pause menu
    pauseMenu->hide(scene);

    // Allow map scrolling
    cursor->setScrollAreaRect(battlefield->getLocation()->mapRectToScene(battlefield->getLocation()->boundingRect()));

    // Resume battlefield events
    battlefield->resume();
}

void GameInterface::processMainMenuClick()
{
    emit mainMenuSignal();
}
