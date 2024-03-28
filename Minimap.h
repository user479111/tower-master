#ifndef MINIMAP_H
#define MINIMAP_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QObject>
#include <QMap>

#include "Tower.h"
#include "Enemy.h"

class Minimap : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Minimap(const QPixmap &pixmap,
            QGraphicsPixmapItem * parent = nullptr);
    ~Minimap();

    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    QPointF getClickPosition() const;

    float getMapScale() const;
    void setMapScale(float newMapScale);

    QPointF getCalculatedScenePosition() const;

    void addTower(Tower * tower);

    void setPos(const QPointF &pos);

    void connectWithEenemies(const QList<Enemy*> &currentEnemies);

    void clearObjects();

signals:
    void mousePressed();

private slots:
    void enemyRepositioned(Enemy * enemy);
    void enemyRemoved(Enemy * enemy);

private:
    QPointF clickPosition;
    QPointF calculatedScenePosition;
    float mapScale;

    QMap<Tower* , QGraphicsRectItem*> towers;
    QMap<Enemy* , QGraphicsRectItem*> enemies;
};

#endif // MINIMAP_H
