#ifndef TOWER_H
#define TOWER_H

#include <QGraphicsPixmapItem>
#include <QSharedPointer>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QList>

#include "Location.h"
#include "Bullet.h"

#define MIN_ATTACK_SPEED 0.1
#define MAX_ATTACK_SPEED 5

class Tower : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Tower(const QString &type,
          const QPointF &center,
          const QSharedPointer<Location> location);
    ~Tower();

    const QString &getType() const;

    const QList<QSharedPointer<Bullet> > &getBullets() const;

    void setAttackSpeed(float newAttackSpeed);

private:
    void loadXmlParameters(QString inFileName);

public slots:
    void fire();
    void aquireTarget();
    void clearBullets();

private:
    QSharedPointer<Location> location;
    QSharedPointer<QGraphicsPolygonItem> attackArea;
    QPointF attackDestination;

    QString type;

    Bullet bulletPatern;
    QList<QSharedPointer<Bullet>> bullets;

    float attackSpeed;
};

#endif // TOWER_H
