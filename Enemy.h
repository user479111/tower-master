#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QGraphicsPathItem>
#include <QSharedPointer>
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
          const QSharedPointer<QGraphicsPathItem> route);
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
    void outOfBattle(int id);

private:
    int id;
    QString type;
    QSharedPointer<QGraphicsPathItem> route;
    int speed;  // pixels per second
    int stepSize;   // step size per timer interval
    int currentDestinationIndex;
    float angle;

    QTimer timerMove;
};

#endif // ENEMY_H
