#include "Minimap.h"

#include <QGraphicsScene>
#include <QList>
#include <QDebug>

Minimap::Minimap(const QPixmap &pixmap,
                 QGraphicsPixmapItem * parent) :
    QGraphicsPixmapItem(pixmap , parent),
    clickPosition(0, 0),
    mapScale(1)
{
}

Minimap::~Minimap()
{
    clearObjects();
}

void Minimap::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    clickPosition = QPointF(event->pos().x() * 0.1 /* move scale to xml */, event->pos().y() * 0.1 /* move scale to xml */);

    // Calculate the point on the map that represents the click point on the minimap
    // X
    if (event->pos().x() * mapScale <= scene()->sceneRect().width() / 2) {
        calculatedScenePosition.setX(scene()->sceneRect().width() / 2);
    }
    else if (event->pos().x() * mapScale >= boundingRect().width() * mapScale - scene()->sceneRect().width() / 2) {
        calculatedScenePosition.setX(boundingRect().width() * mapScale - scene()->sceneRect().width() / 2);
    } else {
        calculatedScenePosition.setX(event->pos().x() * mapScale);
    }
    // Y
    if (event->pos().y() * mapScale <= scene()->sceneRect().height() / 2) {
        calculatedScenePosition.setY(scene()->sceneRect().height() / 2);
    }
    else if (event->pos().y() * mapScale >= boundingRect().height() * mapScale - scene()->sceneRect().height() / 2) {
        calculatedScenePosition.setY(boundingRect().height() * mapScale - scene()->sceneRect().height() / 2);
    } else {
        calculatedScenePosition.setY(event->pos().y() * mapScale);
    }

    emit mousePressed();
}

QPointF Minimap::getClickPosition() const
{
    return clickPosition;
}

float Minimap::getMapScale() const
{
    return mapScale;
}

void Minimap::setMapScale(float newMapScale)
{
    mapScale = newMapScale;
}

QPointF Minimap::getCalculatedScenePosition() const
{
    return calculatedScenePosition;
}

void Minimap::addTower(Tower * tower)
{
    QGraphicsRectItem * towerRect =
            new QGraphicsRectItem(0,
                                  0,
                                  0.1 /* TODO */ * tower->boundingRect().width(),
                                  0.1 /* TODO */ * tower->boundingRect().height());

    towerRect->setPos(x() + tower->pos().x() / tower->scale() * 0.1,
                      y() + tower->pos().y() / tower->scale() * 0.1);
    towerRect->setZValue(1);
    towerRect->setBrush(QBrush(Qt::green));

    scene()->addItem(towerRect);

    towers.insert(tower, towerRect);
}

void Minimap::setPos(const QPointF &pos)
{
    QGraphicsItem::setPos(pos);

    for(auto tower: towers.keys()) {

        if (!tower || !towers.value(tower)) {
            continue;
        }

        towers.value(tower)->setPos(x() + tower->pos().x() / tower->scale() * 0.1,
                                    y() + tower->pos().y() / tower->scale() * 0.1);
    }

    for(auto enemy: enemies.keys()) {

        if (!enemy || !enemies.value(enemy)) {
            continue;
        }

        enemies.value(enemy)->setPos(x() + enemy->pos().x() / enemy->scale() * 0.1,
                                    y() + enemy->pos().y() / enemy->scale() * 0.1);
    }
}

void Minimap::connectWithEenemies(const QList<Enemy*> &currentEnemies)
{
    for (auto enemy : currentEnemies) {
        QGraphicsRectItem * enemyRect =
                new QGraphicsRectItem(0,
                                      0,
                                      0.1 /* TODO */ * enemy->boundingRect().width(),
                                      0.1 /* TODO */ * enemy->boundingRect().height());

        enemyRect->setPos(x() + enemy->pos().x() / enemy->scale() * 0.1,
                          y() + enemy->pos().y() / enemy->scale() * 0.1);
        enemyRect->setZValue(1);

        // Make it visible only if it's within the map
        enemyRect->setPen(QPen(QColor(0, 0, 0, 0)));
        enemyRect->setBrush(QBrush(QColor(255, 0, 0, 0)));

        scene()->addItem(enemyRect);

        enemies.insert(enemy, enemyRect);

        connect(enemy, &Enemy::moved, this, &Minimap::enemyRepositioned);
        connect(enemy, &Enemy::outOfBattleForMinimap, this, &Minimap::enemyRemoved);
    }
}

void Minimap::clearObjects()
{
    for(auto tower: towers.keys()) {
        if (towers.value(tower)) {
            delete towers.value(tower);
        }
    }
    towers.clear();

    for(auto enemy: enemies.keys()) {
        if (enemies.value(enemy)) {
            delete enemies.value(enemy);
        }
    }
    enemies.clear();
}


void Minimap::enemyRepositioned(Enemy * enemy)
{
    enemies.value(enemy)->setPos(x() + enemy->pos().x() / enemy->scale() * 0.1,
                                 y() + enemy->pos().y() / enemy->scale() * 0.1);

    // Show the enemy position rect only if it's within the map
    if (mapRectToScene(boundingRect()).contains(enemies.value(enemy)->pos()) &&
        mapRectToScene(boundingRect()).contains(enemies.value(enemy)->pos() +
                                                enemies.value(enemy)->boundingRect().bottomRight())) {
        enemies.value(enemy)->setPen(QPen(QColor(0, 0, 0, 255)));
        enemies.value(enemy)->setBrush(QBrush(QColor(255, 0, 0, 255)));
    } else {
        foreach (auto sceneItem, scene()->items()) {
            if (sceneItem == enemies.value(enemy)) {
                // Hide enemy
                enemies.value(enemy)->setPen(QPen(QColor(0, 0, 0, 0)));
                enemies.value(enemy)->setBrush(QBrush(QColor(255, 0, 0, 0)));
                break;
            }
        }
    }
}

void Minimap::enemyRemoved(Enemy * enemy)
{
    // Remove the enemy rect position from the 'enemies' map
    // and clear memory for it
    if (enemies.value(enemy)) {
        delete enemies.value(enemy);
        enemies.remove(enemy);
    }
}
