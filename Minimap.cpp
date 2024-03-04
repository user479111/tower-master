#include "Minimap.h"

#include <QList>
#include <QDebug>

Minimap::Minimap(QSharedPointer<QGraphicsScene> scene,
                 const QPixmap &pixmap,
                 QSharedPointer<QGraphicsPixmapItem> parent) :
    QGraphicsPixmapItem(pixmap , parent.get()),
    scene(scene),
    clickPosition(0, 0),
    mapScale(1)
{
}

void Minimap::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    clickPosition = QPointF(event->pos().x() * 0.1 /* move scale to xml */, event->pos().y() * 0.1 /* move scale to xml */);

    // Calculate the point on the map that represents the click point on the minimap
    // X
    if (event->pos().x() * mapScale <= scene->sceneRect().width() / 2) {
        calculatedScenePosition.setX(scene->sceneRect().width() / 2);
    }
    else if (event->pos().x() * mapScale >= boundingRect().width() * mapScale - scene->sceneRect().width() / 2) {
        calculatedScenePosition.setX(boundingRect().width() * mapScale - scene->sceneRect().width() / 2);
    } else {
        calculatedScenePosition.setX(event->pos().x() * mapScale);
    }
    // Y
    if (event->pos().y() * mapScale <= scene->sceneRect().height() / 2) {
        calculatedScenePosition.setY(scene->sceneRect().height() / 2);
    }
    else if (event->pos().y() * mapScale >= boundingRect().height() * mapScale - scene->sceneRect().height() / 2) {
        calculatedScenePosition.setY(boundingRect().height() * mapScale - scene->sceneRect().height() / 2);
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
    QSharedPointer<QGraphicsRectItem>
            towerRect(new QGraphicsRectItem(0,
                                            0,
                                            0.1 /* TODO */ * tower->boundingRect().width(),
                                            0.1 /* TODO */ * tower->boundingRect().height()));

    towerRect->setPos(x() + tower->pos().x() / tower->scale() * 0.1,
                      y() + tower->pos().y() / tower->scale() * 0.1);
    towerRect->setZValue(1);
    towerRect->setBrush(QBrush(Qt::green));

    scene->addItem(towerRect.get());

    towers.insert(tower, towerRect);
}

void Minimap::setPos(const QPointF &pos)
{
    QGraphicsItem::setPos(pos);

    for(auto tower: towers.keys()) {
        towers.value(tower)->setPos(x() + tower->pos().x() / tower->scale() * 0.1,
                                    y() + tower->pos().y() / tower->scale() * 0.1);
    }

    for(auto enemy: enemies.keys()) {
        enemies.value(enemy)->setPos(x() + enemy->pos().x() / enemy->scale() * 0.1,
                                    y() + enemy->pos().y() / enemy->scale() * 0.1);
    }
}

void Minimap::connectWithEenemies(const QList<Enemy*> &currentEnemies)
{
    for (auto enemy : currentEnemies) {
        QSharedPointer<QGraphicsRectItem>
                enemyRect(new QGraphicsRectItem(0,
                                                0,
                                                0.1 /* TODO */ * enemy->boundingRect().width(),
                                                0.1 /* TODO */ * enemy->boundingRect().height()));

        enemyRect->setPos(x() + enemy->pos().x() / enemy->scale() * 0.1,
                          y() + enemy->pos().y() / enemy->scale() * 0.1);
        enemyRect->setZValue(1);
        enemyRect->setBrush(QBrush(Qt::red));

        // Show the enemy position rect only if it's within the map
        if (mapRectToScene(boundingRect()).contains(enemyRect->pos()) &&
            mapRectToScene(boundingRect()).contains(enemyRect->pos() +
                                                    enemyRect->boundingRect().bottomRight())) {
            scene->addItem(enemyRect.get());
        }

        enemies.insert(enemy, enemyRect);

        connect(enemy, &Enemy::moved, this, &Minimap::enemyRepositioned);
        connect(enemy, &Enemy::outOfBattle, this, &Minimap::enemyRemoved);
    }
}

void Minimap::enemyRepositioned(Enemy * enemy)
{
    enemies.value(enemy)->setPos(x() + enemy->pos().x() / enemy->scale() * 0.1,
                                 y() + enemy->pos().y() / enemy->scale() * 0.1);

    // Show the enemy position rect only if it's within the map
    if (mapRectToScene(boundingRect()).contains(enemies.value(enemy)->pos()) &&
        mapRectToScene(boundingRect()).contains(enemies.value(enemy)->pos() +
                                                enemies.value(enemy)->boundingRect().bottomRight())) {

        scene->addItem(enemies.value(enemy).get());

    } else {
        foreach (auto sceneItem, scene->items()) {
            if (sceneItem == enemies.value(enemy).get()) {
                scene->removeItem(enemies.value(enemy).get());
                break;
            }
        }
    }
}

void Minimap::enemyRemoved(Enemy * enemy)
{
    enemies.remove(enemy);
}
