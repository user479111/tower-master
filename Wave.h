#ifndef WAVE_H
#define WAVE_H

#include <QGraphicsScene>
#include <QSharedPointer>
#include <QObject>
#include <QList>
#include <QTimer>

#include "Enemy.h"

class Location;

class Wave : public QObject
{
    Q_OBJECT
public:
    Wave(const QSharedPointer<QGraphicsScene> scene);
    ~Wave();

    int getInterval() const;
    void setInterval(int newInterval);

    float getDencity() const;

    void setDencity(float newDencity);

    void runEnemies(const QSharedPointer<Location> location);
    void setupEnemiesFromXml(QString fileName,
                             const QSharedPointer<Location> location);

    int getId() const;
    void setId(int newId);


    const QList<QSharedPointer<Enemy> > &getGroupOfEnemies() const;

private slots:
    void runEnemy();
    void processEnemyOut(int id);

signals:
    void enemiesEnded();

private:
    int id;         // Number of wave
    int interval;   // Intercal between this and next wave in seconds
    float dencity;  // Dencity of the wawe [0.1 .. 1]

    QList<QSharedPointer<Enemy>> groupOfEnemies;
    int currentEnemyId; // Id of the enemy that is processed
    int enemiesOutOfBattleNum; // Number of enemies that are already out of battle

    QSharedPointer<QGraphicsScene> scene;

    QTimer timerBetweenEnemies;
};

#endif // WAVE_H
