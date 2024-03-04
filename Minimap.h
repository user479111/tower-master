#ifndef MINIMAP_H
#define MINIMAP_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QSharedPointer>
#include <QPointF>
#include <QObject>
#include <QMap>

#include "Tower.h"
#include "Enemy.h"

class Minimap : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Minimap(QSharedPointer<QGraphicsScene> scene,
            const QPixmap &pixmap,
            QSharedPointer<QGraphicsPixmapItem> parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    QPointF getClickPosition() const;

    float getMapScale() const;
    void setMapScale(float newMapScale);

    QPointF getCalculatedScenePosition() const;

    void addTower(Tower * tower);

    void setPos(const QPointF &pos);

    void connectWithEenemies(const QList<Enemy*> &currentEnemies);

signals:
    void mousePressed();

private slots:
    void enemyRepositioned(Enemy * enemy);
    void enemyRemoved(Enemy * enemy);

private:
    QSharedPointer<QGraphicsScene> scene;
    QPointF clickPosition;
    QPointF calculatedScenePosition;
    float mapScale;

    QMap<Tower* , QSharedPointer<QGraphicsRectItem>> towers;
    QMap<Enemy* , QSharedPointer<QGraphicsRectItem>> enemies;
};

#endif // MINIMAP_H
