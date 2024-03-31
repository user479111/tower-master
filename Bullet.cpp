#include "Bullet.h"

#include <QGraphicsScene>
#include <QList>
#include <QDebug>
#include <qmath.h>

Bullet::Bullet() :
    location(nullptr),
    moveTimer(this),
    timerRemainingTimeOnPause(0),
    damage(MIN_BULLET_DAMAGE),
    speed(MIN_BULLET_SPEED),
    outOfBattle(false)
{
    setSuitableForHighlighting(false);
}

Bullet::Bullet(Bullet &bulletPatern) :
    GameObject(),
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
    moveTimer.setInterval(BULLET_TIMER_INTERVAL);

    setPos(pos().x() + stepSize * scale() * cos(qDegreesToRadians(angle)),
           pos().y() + stepSize * scale() * sin(qDegreesToRadians(angle)));

    // Get a list of items colliding with this item
    QList<QGraphicsItem *> collidingItems = this->collidingItems();

    // Iterate through the colliding items list
    for (QGraphicsItem * collidingItem : collidingItems) {
        // Check if the colliding item is the enemy
        auto enemy = dynamic_cast<Enemy *>(collidingItem);

        if (enemy) {

            attackTarget(enemy);

            // Mark the bullet for removal
            outOfBattle = true;
            emit targetReached();

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

void Bullet::pause()
{
    if (moveTimer.isActive()) {
        timerRemainingTimeOnPause = moveTimer.remainingTime();
        moveTimer.stop();
    }
}

void Bullet::resume()
{
    if (timerRemainingTimeOnPause) {
        timerRemainingTimeOnPause = 0;
        moveTimer.start(timerRemainingTimeOnPause);
    }
}

void Bullet::shot()
{
    // Connect timer to move()
    connect(&moveTimer, SIGNAL(timeout()), this, SLOT(move()));
    moveTimer.start(BULLET_TIMER_INTERVAL);
}

void Bullet::attackTarget(Enemy *enemy)
{
    if (enemy->getCurrentHealth() > Enemy::ENEMY_MIN_HEALTH) {
        enemy->setCurrentHealth(enemy->getCurrentHealth() - damage);
    } else {
        enemy->prepareForRemoval();
    }
}

int Bullet::getSpeed() const
{
    return speed;
}

void Bullet::setSpeed(int newSpeed)
{
    speed = (newSpeed < MIN_BULLET_SPEED) ? MIN_BULLET_SPEED : newSpeed;

    stepSize = speed * BULLET_TIMER_INTERVAL / 1000;
}

void Bullet::setScale(qreal scale)
{
    QGraphicsPixmapItem::setScale(this->scale() * scale);
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
