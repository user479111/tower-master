#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QTimer>
#include <QRectF>

#define ENEMY_TIMER_INTERVAL 50
#define ENEMY_MIN_SPEED 20
#define ENEMY_HEALTH_BAR_WIDTH 3

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

    void setPos(const QPointF &pos);

    void prepareForRemoval();

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);

    int getTotalHealth() const;
    void setTotalHealth(int newTotalHealth);

    int getCurrentHealth() const;
    void setCurrentHealth(int newCurrentHealth);

    void pause();
    void resume();

    int getDamage() const;

private:
    void loadXmlParameters(QString inFileName);

    void setRotation(qreal angle);

private slots:
    void moveForward();

signals:
    void outOfBattleForMinimap(Enemy * enemy);
    void outOfBattleForWave(Enemy * enemy);
    void moved(Enemy * enemy);

public:
    static const int ENEMY_MIN_HEALTH;
private:
    int id;
    QString type;
    const QGraphicsPathItem * route;
    int totalHealth;
    int currentHealth;
    int speed;    // Pixels per second
    float stepSize; // Step size per timer interval
    int currentDestinationIndex;
    float angle;
    int damage;

    QTimer moveTimer;
    unsigned int timerRemainingTimeOnPause;

    QGraphicsLineItem * totalHealthBar;
    QGraphicsLineItem * currentHealthBar;
};

#endif // ENEMY_H
