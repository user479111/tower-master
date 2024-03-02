#include "GameInterface.h"
#include "Tower.h"

#include <QDir>
#include <QDebug>

GameInterface::GameInterface(QSharedPointer<QGraphicsScene> scene,
                             QSharedPointer<Cursor> cursor,
                             QSharedPointer<Battlefield> battlefield,
                             QSharedPointer<QObject> parent) :
    QObject(parent.get()),
    scene(scene),
    cursor(cursor),
    battlefield(battlefield),
    pauseMenu(new MenuItem()),
    hidePanels(new MenuItem()),
    buildTowerItem(new MenuItem()),
    currentTowerItem(0),
    scrollForward(new MenuItem()),
    scrollBackward(new MenuItem()),
    hide(false)
{
    qDebug() << "GameInterface::GameInterface";
    cursor->setScrollAreaRect(battlefield->getLocation()->mapRectToScene(battlefield->getLocation()->boundingRect()));

    connect(cursor.get(), &Cursor::areaScrolled, this, &GameInterface::processScroll);

    // show minimap board
    minimapBoard = QSharedPointer<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap(":/Data/Data/Game/MinimapDesk.png"))); // all the paths should be written somewhere
    minimapBoard->setX(scene->sceneRect().right() - minimapBoard->boundingRect().width());
    minimapBoard->setY(scene->sceneRect().bottom() - minimapBoard->boundingRect().height());
    minimapBoard->setZValue(1);
    scene->addItem(minimapBoard.get());

    // show minimap
    minimap = QSharedPointer<Minimap>(new Minimap(scene, QPixmap(battlefield->getLocation()->getLocationImagePath())));
    minimap->setScale(0.1); // TODO: xml
    minimap->setX(minimapBoard->x() +
                    (minimapBoard->boundingRect().width() - minimap->boundingRect().width() * minimap->scale()) / 2);
    minimap->setY(minimapBoard->y() +
                    (minimapBoard->boundingRect().height() - minimap->boundingRect().height() * minimap->scale()) / 2);
    minimap->setZValue(1);
    scene->addItem(minimap.get());

    // process press on minimap
    connect(minimap.get(), &Minimap::mousePressed, this, &GameInterface::processPressEvent);

    // draw shown area on minimap
    shownArea = QSharedPointer<QGraphicsRectItem>(new QGraphicsRectItem(0, 0, scene->sceneRect().width() * 0.1,
                                            scene->sceneRect().height() * 0.1));
    shownArea->setPos(minimap->x() + scene->sceneRect().x()* 0.1,
                      minimap->y() + scene->sceneRect().y()* 0.1);
    shownArea->setZValue(1);
    shownArea->setBrush(Qt::transparent); // Set the fill color
    shownArea->setPen(QPen(Qt::red));
    scene->addItem(shownArea.get());

    // process map scale
    connect(battlefield.get(), &Battlefield::battlefieldScaled, this, &GameInterface::processBattlefieldScale);

    // show player board
    playerBoard = QSharedPointer<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap(":/Data/Data/Game/MinimapDesk.png"))); // all the paths should be written somewhere
    playerBoard->setX(scene->sceneRect().left());
    playerBoard->setY(scene->sceneRect().bottom() - playerBoard->boundingRect().height());
    playerBoard->setZValue(1);
    scene->addItem(playerBoard.get());

    // setup pause menu button
    pauseMenu->setTitle("pause");
    pauseMenu->setPixmap(QString(":/Data/Data/Game/Pause.png"));
    pauseMenu->setPos(QPointF(
        playerBoard->x() + (playerBoard->boundingRect().width() - pauseMenu->boundingRect().width() * 2) / 3,
        playerBoard->y() + pauseMenu->boundingRect().height() * 0.5));
    pauseMenu->setZValue(1);
    scene->addItem(pauseMenu.get());

    connect(pauseMenu.get(), &MenuItem::clicked, this, &GameInterface::processPauseClick);

    // setup hide menu button
    hidePanels->setTitle("hide");
    hidePanels->setPixmap(QString(":/Data/Data/Game/Hide.png"));
    hidePanels->setPos(QPointF(
        playerBoard->x() + playerBoard->boundingRect().width() - hidePanels->boundingRect().width() -
                           (playerBoard->boundingRect().width() - hidePanels->boundingRect().width() * 2) / 3,
        playerBoard->y() + hidePanels->boundingRect().height() / 2));
    hidePanels->setZValue(1);
    scene->addItem(hidePanels.get());

    connect(hidePanels.get(), &MenuItem::clicked, this, &GameInterface::processHideClick);

    // setup build tower icon
    towersTypes = QDir(":/Data/Data/Towers/").entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    buildTowerItem->setTitle("build-tower");
    buildTowerItem->setPixmap(QString(":/Data/Data/Towers/" + towersTypes[currentTowerItem] + "/Tower.png"));
    buildTowerItem->setPos(QPointF(
        playerBoard->x() + playerBoard->boundingRect().width() / 2 - buildTowerItem->boundingRect().width() / 2,
        playerBoard->y() + playerBoard->boundingRect().height() / 2 - buildTowerItem->boundingRect().height() / 2));
    buildTowerItem->setZValue(1);
    buildTowerItem->setStaticSize(true);
    scene->addItem(buildTowerItem.get());

    connect(buildTowerItem.get(), &MenuItem::clicked, this, &GameInterface::processBuildTowerClick);

    scrollForward->setTitle("scroll-forward");
    scrollForward->setPixmap(QString(":/Data/Data/Game/ArrowLeft.png"));
    scrollForward->setPos(QPointF(
        buildTowerItem->x() - scrollForward->boundingRect().width() * 1.1,
        playerBoard->y() + playerBoard->boundingRect().height() / 2 - scrollForward->boundingRect().height() / 2));
    scrollForward->setZValue(1);
    scrollForward->setStaticSize(true);
    scene->addItem(scrollForward.get());

    connect(scrollForward.get(), &MenuItem::clicked, this, &GameInterface::processScrollForward);

    scrollBackward->setTitle("scroll-backward");
    scrollBackward->setPixmap(QString(":/Data/Data/Game/ArrowRight.png"));
    scrollBackward->setPos(QPointF(
        buildTowerItem->x() + buildTowerItem->boundingRect().width() + scrollBackward->boundingRect().width() * 0.1,
        playerBoard->y() + playerBoard->boundingRect().height() / 2 - scrollBackward->boundingRect().height() / 2));
    scrollBackward->setStaticSize(true);
    scrollBackward->setZValue(1);
    scene->addItem(scrollBackward.get());

    connect(scrollBackward.get(), &MenuItem::clicked, this, &GameInterface::processScrollBackward);
}

GameInterface::~GameInterface()
{
    qDebug() << "GameInterface::~GameInterface";
    scene->removeItem(minimapBoard.get());

    scene->removeItem(minimap.get());

    scene->removeItem(shownArea.get());

    scene->removeItem(playerBoard.get());

    scene->removeItem(pauseMenu.get());

    scene->removeItem(hidePanels.get());

    scene->removeItem(buildTowerItem.get());

    scene->removeItem(scrollForward.get());

    scene->removeItem(scrollBackward.get());

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

    int minimapDx = minimap->x();
    int minimapDy = minimap->y();

    minimap->setX(minimapBoard->x() +
                    (minimapBoard->boundingRect().width() - minimap->boundingRect().width() * minimap->scale()) / 2);
    minimap->setY(minimapBoard->y() +
                    (minimapBoard->boundingRect().height() - minimap->boundingRect().height() * minimap->scale()) / 2);

    minimapDx -= minimap->x();
    minimapDy -= minimap->y();

    auto scenePosOnMap = battlefield->getLocation()->mapFromScene(scene->sceneRect().topLeft());

    shownArea->setPos(minimap->x() + scenePosOnMap.x() * 0.1,
                      minimap->y() + scenePosOnMap.y() * 0.1);

    for (auto towerItem: towers) {
        towerItem->setX(towerItem->pos().x() + minimapDx);
        towerItem->setY(towerItem->pos().y() + minimapDy);
    }

    playerBoard->setX(scene->sceneRect().left());
    playerBoard->setY(scene->sceneRect().bottom() - playerBoard->boundingRect().height() +
                      hide * (playerBoard->boundingRect().height() / 2));

    pauseMenu->setPos(QPointF(
        playerBoard->x() + (playerBoard->boundingRect().width() - pauseMenu->boundingRect().width() * 2) / 3,
        playerBoard->y() + pauseMenu->boundingRect().height() * 0.5));

    hidePanels->setPos(QPointF(
        playerBoard->x() + playerBoard->boundingRect().width() - hidePanels->boundingRect().width() -
                           (playerBoard->boundingRect().width() - hidePanels->boundingRect().width() * 2) / 3,
        playerBoard->y() + hidePanels->boundingRect().height() * 0.5));

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

    auto scenePosOnMap = battlefield->getLocation()->mapFromScene(scene->sceneRect().topLeft());

    // Redraw shownArea on minimap
    shownArea->setPos(minimap->x() + scenePosOnMap.x() * 0.1 /* move scale to xml */,
                      minimap->y() + scenePosOnMap.y() * 0.1 /* move scale to xml */);
    shownArea->setScale(
                (minimap->boundingRect().width() * 0.1 /* move scale to xml */ * scene->sceneRect().width() /
                 (battlefield->getLocation()->boundingRect().width() * battlefield->getLocation()->scale())) /
                shownArea->boundingRect().width());

    processScroll();
}

void GameInterface::processPauseClick()
{
    emit mainMenuSignal();
}

void GameInterface::processHideClick()
{
    hide = !hide;

    hidePanels->setPixmap(hide ? QString(":/Data/Data/Game/Show.png") : QString(":/Data/Data/Game/Hide.png"));

    processScroll();
}

void GameInterface::processBuildTowerClick()
{
    cursor->setBuildMode(true, towersTypes[currentTowerItem], battlefield->getLocation()->scale());

    connect(cursor.get(), &Cursor::mousePressed, this, &GameInterface::processBuildingTower);
}

void GameInterface::processScrollForward()
{
    currentTowerItem = (currentTowerItem + 1) % towersTypes.size(); // increment until towersTypes.size()

    buildTowerItem->setPixmap(QString(":/Data/Data/Towers/" + towersTypes[currentTowerItem] + "/Tower.png"));
}

void GameInterface::processScrollBackward()
{
    currentTowerItem = (currentTowerItem > 0 ? currentTowerItem : towersTypes.size()) - 1; // decrement until 0

    buildTowerItem->setPixmap(QString(":/Data/Data/Towers/" + towersTypes[currentTowerItem] + "/Tower.png"));
}

void GameInterface::processBuildingTower()
{
    if (!cursor->getBuildMode() || !cursor->getBuildIsPossible()) {
        return;
    }

    cursor->setBuildMode(false);

    QSharedPointer<Tower> tower(new Tower(towersTypes[currentTowerItem],
                                          cursor->pos(),
                                          battlefield->getLocation()));

    battlefield->addTower(tower);

    QSharedPointer<QGraphicsRectItem>
            towerRect(new QGraphicsRectItem(QRectF(
                                                   minimap->x() + tower->pos().x() * 0.1,
                                                   minimap->y() + tower->pos().y() * 0.1,
                                                   0.1 /* TODO */ * tower->boundingRect().width(),
                                                   0.1 /* TODO */ * tower->boundingRect().height())));

    towerRect->setBrush(QBrush(QColor(0, 255, 0)));
    towerRect->setZValue(1);
    scene->addItem(towerRect.get());
    towers.append(towerRect);

    disconnect(cursor.get(), &Cursor::mousePressed, this, &GameInterface::processBuildingTower);
}
