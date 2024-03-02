#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QSharedPointer>
#include <QObject>
#include <QTimer>

#include "Location.h"

#define MIN_BULLET_DAMAGE 1
#define BULLET_TIMER_INTERVAL 50
#define MIN_BULLET_SPEED 20

class Bullet : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Bullet();
    Bullet(Bullet &Bullet);
    ~Bullet();
    void setAngle(float newAngle);
    bool isOutOfBattle();

    int getDamage() const;
    void setDamage(int newDamage);

    int getSpeed() const;
    void setSpeed(int newSpeed);

    void setLocation(QSharedPointer<Location> newLocation);

    void shot();

    QSharedPointer<Location> getLocation() const;

public slots:
    void move();

signals:
    void targetReached();

private:
    QSharedPointer<Location> location;
    QTimer moveTimer;

    int damage;
    int speed;
    int stepSize;   // step size per timer interval
    float angle;
    bool outOfBattle;
};

#endif // BULLET_H
