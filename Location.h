#ifndef LOCATION_H
#define LOCATION_H

#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QSharedPointer>
#include <QPolygon>
#include <QList>
#include <QPen>

#include "Wave.h"

class Wave;

class Location : public QGraphicsPixmapItem
{
public:
    Location(QString name,
             QSharedPointer<QGraphicsScene> scene);
    ~Location();

    int getWidth() const;

    int getHeight() const;

    const QString &getLocationImagePath() const;

    const QList<QSharedPointer<QGraphicsPolygonItem> > &getBuildAreas() const;

    const QList<QSharedPointer<QGraphicsPathItem> > &getEnemyRoutes() const;

    short getTimeForPreparation() const;

    const QList<QSharedPointer<Wave>> &getWaves() const;

    const QString &getName() const;

private:
    void loadXmlParameters(QString inFileName);

private:
    QString name;
    QString locationImage;
    QString locationImagePath;
    int width;
    int height;
    int timeForPreparation;

    QList<QSharedPointer<QGraphicsPolygonItem>> buildAreas;
    QList<QSharedPointer<QGraphicsPathItem>> enemyRoutes;
    QList<QSharedPointer<Wave>> waves;

    QSharedPointer<QGraphicsScene> scene;
};

#endif // LOCATION_H
