#ifndef TOWER_H
#define TOWER_H

#include <QPointF>
#include <QString>
#include <QList>

#include "GameObject.h"
#include "Location.h"
#include "Bullet.h"

#define MIN_ATTACK_SPEED 0.1
#define MAX_ATTACK_SPEED 5

class Tower : public GameObject
{
    Q_OBJECT
public:
    Tower(const QString &directoryType,
          const QPointF &center,
          const Location * location);
    ~Tower();

    const QString &getType() const;

    const QList<Bullet*> &getBullets() const;

    int getDamage() const;

    float getAttackSpeed() const;
    void setAttackSpeed(float newAttackSpeed);

    void pause();
    void resume();

    const QString &getSkin() const;

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

    QString skin;
    QString type;
    QString directoryType;

    Bullet bulletPatern;
    QList<Bullet*> bullets;

    float attackSpeed;
};

#endif // TOWER_H
