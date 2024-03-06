#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QGraphicsPathItem>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QTimer>

#define ENEMY_TIMER_INTERVAL 50
#define MIN_ENEMY_SPEED 20

class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Enemy(int id,
          const QString &type,
          const QGraphicsPathItem * route);
    ~Enemy();

    const QString &getType() const;
    void setType(const QString &newType);

    int getId() const;

    void run();

    int getSpeed() const;
    void setSpeed(int newSpeed);

    void setScale(qreal scale);

    void prepareForRemoval();

private:
    void loadXmlParameters(QString inFileName);

    void setRotation(qreal angle);

private slots:
    void moveForward();

signals:
    void outOfBattleForMinimap(Enemy * enemy);
    void outOfBattleForWave(Enemy * enemy);
    void moved(Enemy * enemy);

private:
    int id;
    QString type;
    const QGraphicsPathItem * route;
    int speed;    // Pixels per second
    int stepSize; // Step size per timer interval
    int currentDestinationIndex;
    float angle;

    QTimer timerMove;
};

#endif // ENEMY_H
