#ifndef LOCATION_H
#define LOCATION_H

#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QPolygon>
#include <QList>
#include <QPen>

#include "Wave.h"

class Wave;

class Location : public QGraphicsPixmapItem
{
public:
    Location(QGraphicsScene * scene, QString name);
    ~Location();

    int getWidth() const;

    int getHeight() const;

    const QString &getLocationImagePath() const;

    const QList<QGraphicsPolygonItem*> &getBuildAreas() const;

    const QList<QGraphicsPathItem*> &getEnemyRoutes() const;

    short getTimeForPreparation() const;

    const QList<Wave*> &getWaves() const;

    const QString &getName() const;

private:
    void loadXmlParameters(QString inFileName);

private:
    QGraphicsScene * scene;

    QString name;
    QString locationImage;
    QString locationImagePath;
    int width;
    int height;
    int timeForPreparation;

    QList<QGraphicsPolygonItem*> buildAreas;
    QList<QGraphicsPathItem*> enemyRoutes;
    QList<Wave*> waves;
};

#endif // LOCATION_H
