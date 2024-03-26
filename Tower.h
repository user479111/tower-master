#ifndef TOWER_H
#define TOWER_H

#include <QGraphicsPixmapItem>
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
          const Location * location);
    ~Tower();

    const QString &getType() const;

    const QList<Bullet*> &getBullets() const;

    void setAttackSpeed(float newAttackSpeed);

    void pause();
    void resume();

private:
    void loadXmlParameters(QString inFileName);

public slots:
    void fire();
    void aquireTarget();
    void clearBullets();

private:
    const Location * location;

    QTimer attackTimer;
    unsigned int timerRemainingTimeOnPause;

    QPointF attackDestination;
    QGraphicsPolygonItem * attackArea;

    QString type;

    Bullet bulletPatern;
    QList<Bullet*> bullets;

    float attackSpeed;
};

#endif // TOWER_H
