#include "Bullet.h"

#include <QGraphicsScene>
#include <QList>
#include <QDebug>
#include <qmath.h>

Bullet::Bullet() :
    location(nullptr),
    damage(MIN_BULLET_DAMAGE),
    speed(MIN_BULLET_SPEED),
    outOfBattle(false)
{
}

Bullet::Bullet(Bullet &bulletPatern) :
    outOfBattle(false)
{
    setPixmap(bulletPatern.pixmap());
    setLocation(bulletPatern.getLocation());
    setDamage(bulletPatern.getDamage());
    setSpeed(bulletPatern.getSpeed());
}

Bullet::~Bullet()
{
}

void Bullet::move()
{
    setPos(pos().x() + stepSize * cos(qDegreesToRadians(angle)),
           pos().y() + stepSize * sin(qDegreesToRadians(angle)));

    // Get a list of items colliding with this item
    QList<QGraphicsItem *> collidingItems = this->collidingItems();

    // Iterate through the list
    for (QGraphicsItem * collidingItem : collidingItems) {
        // Check if the colliding item is the enemy
        auto enemy = dynamic_cast<Enemy *>(collidingItem);

        if (enemy) {
            // Mark the bullet for removal
            outOfBattle = true;
            emit targetReached();

            // Same for the enemy
            enemy->prepareForRemoval();

            break; // No need to continue checking once intersection is detected
        }
    }

    // Mark the bullet if it crossed the scene bounds
    outOfBattle = location->boundingRect().left() > pos().x() ||
            location->boundingRect().right() * location->scale() < pos().x() ||
            location->boundingRect().top() > pos().y() ||
            location->boundingRect().bottom() * location->scale() < pos().y();
}

const Location *Bullet::getLocation() const
{
    return location;
}

void Bullet::setLocation(const Location *newLocation)
{
    location = newLocation;
}

void Bullet::shot()
{
    // Connect timer to move()
    connect(&moveTimer, SIGNAL(timeout()), this, SLOT(move()));
    moveTimer.start(ENEMY_TIMER_INTERVAL);
}

int Bullet::getSpeed() const
{
    return speed;
}

void Bullet::setSpeed(int newSpeed)
{
    if (newSpeed < MIN_BULLET_SPEED) {
        speed = MIN_BULLET_SPEED;
    } else {
        speed = newSpeed;
    }

    stepSize = speed * BULLET_TIMER_INTERVAL / 1000;
}

int Bullet::getDamage() const
{
    return damage;
}

void Bullet::setDamage(int newDamage)
{
    if (newDamage < MIN_BULLET_DAMAGE) {
        damage = MIN_BULLET_DAMAGE;
    } else {
        damage = newDamage;
    }
}

void Bullet::setAngle(float newAngle)
{
    angle = newAngle;
}

bool Bullet::isOutOfBattle()
{
    return outOfBattle;
}
