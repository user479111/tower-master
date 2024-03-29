#include "GameInterface.h"
#include "Tower.h"

#include <QDir>
#include <QDebug>

const float GameInterface::BUILD_ITEM_SCALE = 0.8;
const float GameInterface::BOARD_ITEM_SCALE = 0.5;
const float GameInterface::MINIMAP_SCALE = 0.1;

const QString GameInterface::FONT_STYLE = "Helvetica [Cronyx]";
const int GameInterface::FONT_SIZE = 9;

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
    largestTowerWidth(0),
    scrollForward(new MenuItem()),
    scrollBackward(new MenuItem()),
    totalBaseHealthBar(new QGraphicsRectItem(0, 0, 0, 0)),
    currentBaseHealthBar(new QGraphicsRectItem(0, 0, 0, 0)),
    healthInfo(new QGraphicsTextItem),
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
    minimap->setScale(MINIMAP_SCALE);
    minimap->setX(minimapBoard->x() +
                    (minimapBoard->boundingRect().width() - minimap->boundingRect().width() * minimap->scale()) / 2);
    minimap->setY(minimapBoard->y() +
                    (minimapBoard->boundingRect().height() - minimap->boundingRect().height() * minimap->scale()) / 2);
    minimap->setZValue(1);
    scene->addItem(minimap);

    // process press on minimap
    connect(minimap, &Minimap::mousePressed, this, &GameInterface::processPressEvent);

    // draw shown area on minimap
    shownArea = new QGraphicsRectItem(0, 0, scene->sceneRect().width() * MINIMAP_SCALE,
                                            scene->sceneRect().height() * MINIMAP_SCALE);
    shownArea->setPos(minimap->x() + scene->sceneRect().x()* MINIMAP_SCALE,
                      minimap->y() + scene->sceneRect().y()* MINIMAP_SCALE);
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

    pauseMenuItem->setScale(BOARD_ITEM_SCALE);
    pauseMenuItem->setPos(QPointF(
        playerBoard->x() + (playerBoard->boundingRect().width() -
                               pauseMenuItem->boundingRect().width() * 2 *
                               pauseMenuItem->scale()) / 3,
        playerBoard->y() + pauseMenuItem->boundingRect().height() * pauseMenuItem->scale() * 1.5));
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

    hidePanels->setScale(BOARD_ITEM_SCALE);
    hidePanels->setPos(QPointF(
        playerBoard->x() + playerBoard->boundingRect().width() - hidePanels->boundingRect().width() * hidePanels->scale() -
                           (playerBoard->boundingRect().width() - hidePanels->boundingRect().width() * hidePanels->scale() * 2) / 3,
        playerBoard->y() + hidePanels->boundingRect().height() * hidePanels->scale() * 1.5));
    hidePanels->setZValue(1);
    hidePanels->show(scene);

    connect(hidePanels, &MenuItem::clicked, this, &GameInterface::processHideClick);

    // Setup total health bar for base
    totalBaseHealthBar->setRect(QRectF(0, 0,
                                       hidePanels->pos().x() + hidePanels->boundingRect().width() * hidePanels->scale() - pauseMenuItem->pos().x(),
                                       pauseMenuItem->boundingRect().height() * pauseMenuItem->scale() / 2));
    totalBaseHealthBar->setPos(pauseMenuItem->pos().x(),
                               playerBoard->y() +
                               pauseMenuItem->boundingRect().height() * pauseMenuItem->scale() / 2);
    totalBaseHealthBar->setZValue(1);
    totalBaseHealthBar->setBrush(QBrush(Qt::red));

    scene->addItem(totalBaseHealthBar);

    // Setup current health bar for base
    currentBaseHealthBar->setRect(QRectF(0, 0,
                                       hidePanels->pos().x() + hidePanels->boundingRect().width() * hidePanels->scale() - pauseMenuItem->pos().x(),
                                       pauseMenuItem->boundingRect().height() * pauseMenuItem->scale() / 2));
    currentBaseHealthBar->setPos(totalBaseHealthBar->pos().x() +
                                 totalBaseHealthBar->boundingRect().width() / 2 -
                                 currentBaseHealthBar->boundingRect().width() / 2,
                                 totalBaseHealthBar->pos().y());
    currentBaseHealthBar->setZValue(1);
    currentBaseHealthBar->setBrush(QBrush(Qt::green));

    scene->addItem(currentBaseHealthBar);

    // Setup health info
    healthInfo->setPlainText(QString::number(battlefield->getEnemyDamageGoal() - battlefield->getEnemyReachedNumber())
                             + '/' + QString::number(battlefield->getEnemyDamageGoal()));
    healthInfo->setDefaultTextColor(Qt::black);
    healthInfo->setFont(QFont(FONT_STYLE, FONT_SIZE, QFont::Bold));
    healthInfo->setX(currentBaseHealthBar->x() +
                     currentBaseHealthBar->boundingRect().width() / 2 -
                     healthInfo->boundingRect().width() / 2);
    healthInfo->setY(currentBaseHealthBar->y() +
                     currentBaseHealthBar->boundingRect().height() / 2 -
                     healthInfo->boundingRect().height() / 2);
    healthInfo->setZValue(1);

    scene->addItem(healthInfo);

    connect(battlefield, &Battlefield::enemyCausedDamage, this, &GameInterface::processEnemyAttack);

    // setup build tower icon
    towersTypes = QDir(":/Data/Data/Towers/").entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    buildTowerItem->setTitle("build-tower");
    buildTowerItem->setPixmap(QString(":/Data/Data/Towers/" + towersTypes[currentTowerItem] + "/Tower.png"));
    buildTowerItem->setScale(BUILD_ITEM_SCALE);

    // Place it in the middle of the 'playerBoard' on horizontal line
    buildTowerItem->setX(playerBoard->x() +
                         playerBoard->boundingRect().width() / 2 -
                         buildTowerItem->boundingRect().width() * buildTowerItem->scale() / 2);

    // Place it in the middle between 'pauseMenuItem' and the bottom of 'playerBoard'
    buildTowerItem->setY(pauseMenuItem->pos().y() +
                         (playerBoard->y() + playerBoard->boundingRect().height() -
                         pauseMenuItem->pos().y() +
                         pauseMenuItem->boundingRect().height() * pauseMenuItem->scale()) / 2 -
                         buildTowerItem->boundingRect().height() * buildTowerItem->scale() / 2);
    buildTowerItem->setZValue(1);
    buildTowerItem->setStaticSize(true);
    scene->addItem(buildTowerItem);

    connect(buildTowerItem, &MenuItem::clicked, this, &GameInterface::processBuildTowerClick);

    // Prepare 'largestTowerWidth'
    for (size_t i = 0, towerTypesNum = towersTypes.size(); i != towerTypesNum; i++) {

        QPixmap tmpTowerBuildItem(QString(":/Data/Data/Towers/" + towersTypes[currentTowerItem] + "/Tower.png"));
        if (largestTowerWidth < tmpTowerBuildItem.width() * BUILD_ITEM_SCALE) {
            largestTowerWidth = tmpTowerBuildItem.width() * BUILD_ITEM_SCALE;
        }

    }

    // Setup 'scrollBackward' button
    scrollBackward->setTitle("scroll-backward");
    scrollBackward->setPixmap(QString(":/Data/Data/Game/ArrowLeft.png"));

    // Place it between the 'buildTowerItem' and the left border of the board
    scrollBackward->setX(playerBoard->x() +
                         (playerBoard->boundingRect().width() / 2 - largestTowerWidth / 2) / 2 -
                         scrollBackward->boundingRect().width() / 2);

    scrollBackward->setY(pauseMenuItem->pos().y() +
                        (playerBoard->y() + playerBoard->boundingRect().height() -
                        pauseMenuItem->pos().y() +
                        pauseMenuItem->boundingRect().height() * pauseMenuItem->scale()) / 2 -
                        scrollBackward->boundingRect().height() * scrollBackward->scale() / 2);
    scrollBackward->setZValue(1);
    scrollBackward->setStaticSize(true);
    scene->addItem(scrollBackward);

    connect(scrollBackward, &MenuItem::clicked, this, &GameInterface::processScrollBackward);

    // setup 'scrollForward' button
    scrollForward->setTitle("scroll-forward");
    scrollForward->setPixmap(QString(":/Data/Data/Game/ArrowRight.png"));

    // Place it between the 'buildTowerItem' and the right border of the board
    scrollForward->setX(playerBoard->x() + playerBoard->boundingRect().width() * 3 / 4 +
                        largestTowerWidth / 4 - scrollForward->boundingRect().width() / 2);

    scrollForward->setY(pauseMenuItem->pos().y() +
                        (playerBoard->y() + playerBoard->boundingRect().height() -
                        pauseMenuItem->pos().y() +
                        pauseMenuItem->boundingRect().height() * pauseMenuItem->scale()) / 2 -
                        scrollForward->boundingRect().height() * scrollForward->scale() / 2);
    scrollForward->setStaticSize(true);
    scrollForward->setZValue(1);
    scene->addItem(scrollForward);

    connect(scrollForward, &MenuItem::clicked, this, &GameInterface::processScrollForward);

    connect(battlefield, &Battlefield::enemiesHaveBeenRun, this, &GameInterface::connectMinimapWithEnemies);

    connect(pauseMenu, &PauseMenu::resumeClicked, this, &GameInterface::processResumeClick);
    connect(pauseMenu, &PauseMenu::restartClicked, this, &GameInterface::processRestartClick);
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

    delete totalBaseHealthBar;

    delete currentBaseHealthBar;

    delete healthInfo;

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
    shownArea->setPos(minimap->pos() + scenePosOnMap * MINIMAP_SCALE);

    playerBoard->setX(scene->sceneRect().left());
    playerBoard->setY(scene->sceneRect().bottom() - playerBoard->boundingRect().height() +
                      hide * (playerBoard->boundingRect().height() / 2));

    pauseMenuItem->setPos(QPointF(
        playerBoard->x() + (playerBoard->boundingRect().width() -
                            pauseMenuItem->boundingRect().width() *
                            pauseMenuItem->scale() * 2) / 3,
        playerBoard->y() + pauseMenuItem->boundingRect().height() *
                          pauseMenuItem->scale() * 1.5));

    hidePanels->setPos(QPointF(
        playerBoard->x() + playerBoard->boundingRect().width() -
                           hidePanels->boundingRect().width() *
                           hidePanels->scale() -
                           (playerBoard->boundingRect().width() -
                            hidePanels->boundingRect().width() *
                            hidePanels->scale() * 2) / 3,
        playerBoard->y() + hidePanels->boundingRect().height() *
                           hidePanels->scale() * 1.5));

    totalBaseHealthBar->setPos(QPointF(pauseMenuItem->pos().x(),
                                         playerBoard->y() +
                                         pauseMenuItem->boundingRect().height() * pauseMenuItem->scale() / 2));

    currentBaseHealthBar->setPos(totalBaseHealthBar->pos().x() +
                                 totalBaseHealthBar->boundingRect().width() / 2 -
                                 currentBaseHealthBar->boundingRect().width() / 2,
                                 totalBaseHealthBar->pos().y());

    healthInfo->setX(currentBaseHealthBar->x() +
                     currentBaseHealthBar->boundingRect().width() / 2 -
                     healthInfo->boundingRect().width() / 2);
    healthInfo->setY(currentBaseHealthBar->y() +
                     currentBaseHealthBar->boundingRect().height() / 2 -
                     healthInfo->boundingRect().height() / 2);

    // Place it in the middle of the 'playerBoard' on horizontal line
    buildTowerItem->setX(playerBoard->x() +
                         playerBoard->boundingRect().width() / 2 -
                         buildTowerItem->boundingRect().width() * buildTowerItem->scale() / 2);

    // Place it in the middle between 'pauseMenuItem' and the bottom of 'playerBoard'
    buildTowerItem->setY(pauseMenuItem->pos().y() +
                         (playerBoard->y() + playerBoard->boundingRect().height() -
                         pauseMenuItem->pos().y() +
                         pauseMenuItem->boundingRect().height() * pauseMenuItem->scale()) / 2 -
                         buildTowerItem->boundingRect().height() * buildTowerItem->scale() / 2);

    // Place it between the 'buildTowerItem' and the left border of the board
    scrollBackward->setX(playerBoard->x() +
                         (playerBoard->boundingRect().width() / 2 - largestTowerWidth / 2) / 2 -
                         scrollBackward->boundingRect().width() / 2);

    scrollBackward->setY(pauseMenuItem->pos().y() +
                        (playerBoard->y() + playerBoard->boundingRect().height() -
                        pauseMenuItem->pos().y() +
                        pauseMenuItem->boundingRect().height() * pauseMenuItem->scale()) / 2 -
                        scrollBackward->boundingRect().height() * scrollBackward->scale() / 2);

    // Place it between the 'buildTowerItem' and the right border of the board
    scrollForward->setX(playerBoard->x() + playerBoard->boundingRect().width() * 3 / 4 +
                        largestTowerWidth / 4 - scrollForward->boundingRect().width() / 2);

    scrollForward->setY(pauseMenuItem->pos().y() +
                        (playerBoard->y() + playerBoard->boundingRect().height() -
                        pauseMenuItem->pos().y() +
                        pauseMenuItem->boundingRect().height() * pauseMenuItem->scale()) / 2 -
                        scrollForward->boundingRect().height() * scrollForward->scale() / 2);
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
    shownArea->setPos(minimap->pos() + scenePosOnMap * MINIMAP_SCALE);
    shownArea->setScale(
                (minimap->boundingRect().width() * MINIMAP_SCALE * scene->sceneRect().width() /
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
    buildTowerItem->setScale(BUILD_ITEM_SCALE);

    // Set the Icon on the updated position
    // Place it in the middle of the 'playerBoard' on horizontal line
    buildTowerItem->setX(playerBoard->x() +
                         playerBoard->boundingRect().width() / 2 -
                         buildTowerItem->boundingRect().width() * buildTowerItem->scale() / 2);

    // Place it in the middle between 'pauseMenuItem' and the bottom of 'playerBoard'
    buildTowerItem->setY(pauseMenuItem->pos().y() +
                         (playerBoard->y() + playerBoard->boundingRect().height() -
                         pauseMenuItem->pos().y() +
                         pauseMenuItem->boundingRect().height() * pauseMenuItem->scale()) / 2 -
                         buildTowerItem->boundingRect().height() * buildTowerItem->scale() / 2);
}

void GameInterface::processScrollBackward()
{
    currentTowerItem = (currentTowerItem > 0 ? currentTowerItem : towersTypes.size()) - 1; // decrement until 0

    buildTowerItem->setPixmap(QString(":/Data/Data/Towers/" + towersTypes[currentTowerItem] + "/Tower.png"));
    buildTowerItem->setScale(BUILD_ITEM_SCALE);

    // Set the Icon on the updated position
    // Place it in the middle of the 'playerBoard' on horizontal line
    buildTowerItem->setX(playerBoard->x() +
                         playerBoard->boundingRect().width() / 2 -
                         buildTowerItem->boundingRect().width() * buildTowerItem->scale() / 2);

    // Place it in the middle between 'pauseMenuItem' and the bottom of 'playerBoard'
    buildTowerItem->setY(pauseMenuItem->pos().y() +
                         (playerBoard->y() + playerBoard->boundingRect().height() -
                         pauseMenuItem->pos().y() +
                         pauseMenuItem->boundingRect().height() * pauseMenuItem->scale()) / 2 -
                         buildTowerItem->boundingRect().height() * buildTowerItem->scale() / 2);
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

void GameInterface::processRestartClick()
{
    emit restart();
}

void GameInterface::processMainMenuClick()
{
    emit mainMenuSignal();
}

void GameInterface::processEnemyAttack()
{
    // Update health information
    healthInfo->setPlainText(QString::number(battlefield->getEnemyDamageGoal() - battlefield->getEnemyReachedNumber())
                             + '/' + QString::number(battlefield->getEnemyDamageGoal()));

    // Shorten health bar
    currentBaseHealthBar->setRect(QRectF(0, 0,
                                       (totalBaseHealthBar->boundingRect().width() *
                                        (battlefield->getEnemyDamageGoal() - battlefield->getEnemyReachedNumber())) /
                                         battlefield->getEnemyDamageGoal(),
                                       totalBaseHealthBar->boundingRect().height()));

    // Place it on the middle of total health bar
    currentBaseHealthBar->setPos(totalBaseHealthBar->pos().x() +
                                 totalBaseHealthBar->boundingRect().width() / 2 -
                                 currentBaseHealthBar->boundingRect().width() / 2,
                                 totalBaseHealthBar->pos().y());
}
