#include "Battlefield.h"

#include <QGraphicsSceneWheelEvent>
#include <QtMath>
#include <QRectF>
#include <QLabel>
#include <QDebug>

Battlefield::Battlefield(QGraphicsScene * scene,
                         Cursor * cursor,
                         Level * level) :
    scene(scene),
    cursor(cursor),
    level(level),
    timerBetweenWaves(this),
    timerRemainingTimeOnPause(0),
    currentWaveIndex(0),
    minScale(0.0),
    paused(false),
    levelCompleted(false),
    enemyReachedNumber(0)
{
    // display location
    scene->addItem(level->getLocation());

    // Temporarily dysplay polygons
    for (auto polygonItem: level->getLocation()->getBuildAreas()) {

        // Set the pen color and style
        QPen pen(Qt::red);              // Set pen color to red
        pen.setStyle(Qt::DashDotLine);  // Set pen style to dash line
        polygonItem->setPen(pen);       // Apply the pen to the polygon item

        // Set the brush color and transparency
        QBrush brush(QColor(255, 0, 0, 128));   // Set brush color to red with 50% transparency
        polygonItem->setBrush(brush);           // Apply the brush to the polygon item

        scene->addItem(polygonItem);
    }

    // Temporarily dysplay enemy routes
    for (auto routeItem: level->getLocation()->getEnemyRoutes()) {

        // Set the pen color and style
        QPen pen(Qt::blue);         // Set pen color to red
        pen.setStyle(Qt::DotLine);  // Set pen style to dash line
        routeItem->setPen(pen);     // Apply the pen to the polygon item

        scene->addItem(routeItem);
    }

    scene->installEventFilter(this);

    minScale = level->getLocation()->boundingRect().height() < level->getLocation()->boundingRect().width() ?
                scene->sceneRect().height() / level->getLocation()->boundingRect().height() :
                scene->sceneRect().width() / level->getLocation()->boundingRect().width();

    connect(cursor, &Cursor::cursorMoved, this, &Battlefield::processCursorMove);

    // Set the interval to the time for preparation in milliseconds
    timerBetweenWaves.setInterval(level->getLocation()->getTimeForPreparation() * 1000);

    // Start waves move on timeout
    QObject::connect(&timerBetweenWaves, &QTimer::timeout, this, &Battlefield::startWaveMove);

    // Start the timer
    timerBetweenWaves.start();
}

Battlefield::~Battlefield()
{
    clearTowers();
}

bool Battlefield::eventFilter(QObject *obj, QEvent *event) {

    if (obj == scene && event->type() == QEvent::GraphicsSceneWheel && !paused) {

        QGraphicsSceneWheelEvent *wheelEvent = static_cast<QGraphicsSceneWheelEvent*>(event);

        qreal scaleFactor = 1.0 + (wheelEvent->delta() / 1200.0); // Calculate scale factor

        auto sceneRect = scene->sceneRect();
        auto sceneCenterPointBeforeScale = level->getLocation()->mapFromScene(sceneRect.center());

        // Scale location
        if ((level->getLocation()->scale() * scaleFactor >= minScale &&      // scene rect shouldn't be bigger than map
                scaleFactor < 1) ||                              // allow increase
                (level->getLocation()->scale() * scaleFactor <= MAX_SCALE && // scene rect shouldn't be too small
                scaleFactor > 1))                                // allow decrease
        {
            // Scale the location based on the wheel scroll
            level->getLocation()->setScale(level->getLocation()->scale() * scaleFactor);

            // Scale build areas
            for (auto area: level->getLocation()->getBuildAreas()) {
                area->setScale(level->getLocation()->scale());
            }

            // Scale enemy routes
            for (auto route: level->getLocation()->getEnemyRoutes()) {
                route->setScale(level->getLocation()->scale());
            }

            // Scale all the built towers
            for (auto towerItem : towers) {

                if (!towerItem) {
                    continue;
                }

                towerItem->setScale(level->getLocation()->scale());

                // Place tower on the "scaled" position on the location
                towerItem->setPos(towerItem->pos() * scaleFactor);

                // Same for each bullet
                for (auto bulletItem : towerItem->getBullets()) {

                    if (!bulletItem) {
                        continue;
                    }

                    bulletItem->setScale(scaleFactor);
                    bulletItem->setPos(bulletItem->pos() * scaleFactor);
                }

            }

            // Scale all the enemies on the battlefield
            for (auto enemyItem : getGroupOfEnemies())
            {
                if (!enemyItem) {
                    continue;
                }

                enemyItem->setScale(scaleFactor);
            }

        } else {
            return true; // Event handled
        }

        // Get the scene center point after the location scaling
        auto sceneCenterPointAfterScale = level->getLocation()->mapToScene(sceneCenterPointBeforeScale);

        // Calculate scene rect center translation
        int dx = sceneCenterPointAfterScale.x() - sceneRect.center().x(),
            dy = sceneCenterPointAfterScale.y() - sceneRect.center().y();

        // Get location rect coordinates relative to the scene coordinate system
        auto locationRect = level->getLocation()->mapRectToScene(level->getLocation()->boundingRect());

        // Do not allow the scene to be out of map's borders
        if (sceneRect.top() < locationRect.top()) {
            dy += locationRect.top() - sceneRect.top();
        }
        if (sceneRect.left() < locationRect.left()) {
            dx += locationRect.left() - sceneRect.left();
        }
        if (sceneRect.bottom() > locationRect.bottom()) {
            dy += locationRect.bottom() - sceneRect.bottom();
        }
        if (sceneRect.right() > locationRect.right()) {
            dx += locationRect.right() - sceneRect.right();
        }

        sceneRect.translate(dx, dy);
        scene->setSceneRect(sceneRect);

        // Keep scaling for the cursor in build mode
        if (cursor->getBuildMode()) {
            cursor->setScale(level->getLocation()->scale());
            cursor->setPos(QPointF(cursor->pos().x() + dx + cursor->pixmap().width() / 2,
                                   cursor->pos().y() + dy + cursor->pixmap().height() / 2));
        } else {
            // Make sure the cursor stays at the same place on the screen
            cursor->setPos(QPointF(cursor->pos().x() + dx, cursor->pos().y() + dy));
        }

        // Update corresponding cursor parameters
        cursor->setScrollAreaRect(level->getLocation()->mapRectToScene(level->getLocation()->boundingRect()));

        emit battlefieldScaled(); // return the signal to the GameInterface

        return true; // Event handled

    } else {
        // Pass the event to the base class for default behavior
        return QObject::eventFilter(obj, event);
    }

}

void Battlefield::addTower(Tower * newTower)
{
    towers.append(newTower);
    newTower->show(scene);
}

const QList<Enemy*> &Battlefield::getGroupOfEnemies() const
{
    return level->getLocation()->getWaves().at(currentWaveIndex)->getGroupOfEnemies();
}

void Battlefield::processCursorMove()
{
    if(!cursor->getBuildMode()) {
        return;
    }

    // Get the bounding rectangle of the pixmap item
    auto cursorRect = cursor->mapRectToScene(cursor->boundingRect());

    bool containsPixmap = false;

    // Check if any point of the cursor bounding rectangle is contained within the build areas
    for (auto polygonItem: level->getLocation()->getBuildAreas()) {

        // Calculate the resulting polygon after scaling
        QPolygonF scaledPolygon;
        for (const auto& point : polygonItem->polygon()) {
            scaledPolygon << QPointF(point.x() * level->getLocation()->scale(),
                                     point.y() * level->getLocation()->scale());
        }
        QGraphicsPolygonItem poly(scaledPolygon);

        if (poly.contains(cursorRect.topLeft()) && poly.contains(cursorRect.topRight()) &&
            poly.contains(cursorRect.bottomRight()) && poly.contains(cursorRect.bottomLeft()))
        {
            containsPixmap = true;
            break;
        }

    }

    // Check if current position isn't occupied by other tower
    for (auto towerItem: towers) {

        auto towerItemRect = towerItem->mapRectToScene(towerItem->boundingRect());

        if (cursorRect.intersects(towerItemRect))
        {
            containsPixmap = false;
            break;
        }
    }

    cursor->setBuildImage(containsPixmap);
}

void Battlefield::startWaveMove()
{
    // Stop timer
    timerBetweenWaves.stop();

    QObject::disconnect(&timerBetweenWaves, &QTimer::timeout, this, &Battlefield::startWaveMove);

    // Setup enemies for the wave
    level->getLocation()->getWaves().at(currentWaveIndex)->runEnemies(level->getLocation());

    // Prepare for enemy attack
    QObject::connect(level->getLocation()->getWaves().at(currentWaveIndex),
                     &Wave::enemyAttacksTheBase,
                     this,
                     &Battlefield::processEnemyAttack);

    for (auto enemy : getGroupOfEnemies()) {
        connect(enemy, &GameObject::clicked, this, &Battlefield::processEnemyClicked);
        connect(enemy, &Enemy::outOfBattleForBattlefield, this, &Battlefield::processHighlightedEnemyOut);
        connect(enemy, &Enemy::healthDecreased, this, &Battlefield::processHighlightedEnemyUpdate);
    }

    // After the last enemy was killed
    // or reached the end of the route
    // startNextWave
    QObject::connect(level->getLocation()->getWaves().at(currentWaveIndex),
                     &Wave::enemiesEnded,
                     this,
                     &Battlefield::startNextWave);

    // Start the timer
    timerBetweenWaves.start(level->getLocation()->getTimeForPreparation() * 1000);

    // Emit signal to connect enemies with minimap
    emit enemiesHaveBeenRun();
}

void Battlefield::startNextWave()
{
    if (paused) {
        // If the timer has managed to start right after the pasuse
        // make sure to start it on resume
        timerRemainingTimeOnPause = level->getLocation()->getTimeForPreparation() * 1000;
        timerBetweenWaves.stop();
        return;
    }

    // Stop timer
    timerBetweenWaves.stop();

    QObject::disconnect(&timerBetweenWaves, &QTimer::timeout, this, &Battlefield::startNextWave);

    // run the next wave
    timerBetweenWaves.setInterval(level->getLocation()->getWaves().at(currentWaveIndex)->getInterval() * 1000);

    if (currentWaveIndex + 1 == level->getLocation()->getWaves().size()) {
        levelCompleted = true;
        emit victory();
        return;
    } else {
        currentWaveIndex++;
    }

    // After a pause
    // Start waves move on timeout
    QObject::connect(&timerBetweenWaves, &QTimer::timeout, this, &Battlefield::startWaveMove);

    // Start the timer
    timerBetweenWaves.start(level->getLocation()->getTimeForPreparation() * 1000);
}

void Battlefield::processEnemyAttack(const int &damage)
{
    if (enemyReachedNumber + damage < getEnemyDamageGoal()) {
        enemyReachedNumber += damage;
        emit enemyCausedDamage();
    } else {
        enemyReachedNumber = getEnemyDamageGoal();
        emit enemyCausedDamage();
        emit gameOver();
    }
}

void Battlefield::processEnemyClicked(const GameObject *object)
{
    emit enemyClicked(object);
}

void Battlefield::processHighlightedEnemyOut()
{
    emit highlightedEnemyOut();
}

void Battlefield::processHighlightedEnemyUpdate(const Enemy *enemy)
{
    emit highlightedEnemyUpdate(enemy);
}

bool Battlefield::getLevelCompleted() const
{
    return levelCompleted;
}

void Battlefield::setLevelCompleted(bool newLevelCompleted)
{
    levelCompleted = newLevelCompleted;
}

Level *Battlefield::getLevel() const
{
    return level;
}

const QList<Tower *> &Battlefield::getTowers() const
{
    return towers;
}

void Battlefield::updateGameObjectsHighlighting()
{
    for (auto tower : towers) {
        tower->setHighlighting();
    }
    for (auto enemy : getGroupOfEnemies()) {
        enemy->setHighlighting();
    }
}

bool Battlefield::getPaused() const
{
    return paused;
}

int Battlefield::getEnemyReachedNumber() const
{
    return enemyReachedNumber;
}

int Battlefield::getEnemyDamageGoal() const
{
    return level->getLocation()->getEnemyDamageGoal();
}

void Battlefield::clearTowers()
{
    for (auto tower : towers) {
        delete tower;
    }
    towers.clear();
}

void Battlefield::pause()
{
    paused = true;

    // Stop towers' timers
    for (auto tower : towers) {
        tower->pause();
    }

    // Stop wave
    level->getLocation()->getWaves().at(currentWaveIndex)->pause();

    if (timerBetweenWaves.isActive()) {
        timerRemainingTimeOnPause = timerBetweenWaves.remainingTime();
        timerBetweenWaves.stop();
    }
}

void Battlefield::resume()
{
    paused = false;

    // Resume towers' timers
    for (auto tower : towers) {
        tower->resume();
    }

    // Resume wave
    level->getLocation()->getWaves().at(currentWaveIndex)->resume();

    if (timerRemainingTimeOnPause > 0) {
        timerBetweenWaves.start(timerRemainingTimeOnPause);
        timerRemainingTimeOnPause = 0;
    } else {
        timerBetweenWaves.start();
    }
}

Location *Battlefield::getLocation() const
{
    return level->getLocation();
}
