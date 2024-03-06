#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
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

    void shot();

    const Location *getLocation() const;
    void setLocation(const Location *newLocation);

public slots:
    void move();

signals:
    void targetReached();

private:
    const Location * location;
    QTimer moveTimer;

    int damage;
    int speed;
    int stepSize;   // step size per timer interval
    float angle;
    bool outOfBattle;
};

#endif // BULLET_H
