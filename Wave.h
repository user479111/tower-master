#ifndef WAVE_H
#define WAVE_H

#include <QGraphicsScene>
#include <QObject>
#include <QList>
#include <QTimer>

#include "Enemy.h"

class Location;

class Wave : public QObject
{
    Q_OBJECT
public:
    Wave(QGraphicsScene * scene);
    ~Wave();

    int getInterval() const;
    void setInterval(int newInterval);

    float getDencity() const;

    void setDencity(float newDencity);

    void runEnemies(const Location * location);
    void setupEnemiesFromXml(QString fileName,
                             const Location * location);

    int getId() const;
    void setId(int newId);

    const QList<Enemy*> &getGroupOfEnemies() const;

    void pause();
    void resume();

    void clearEnemies();

private slots:
    void runEnemy();
    void processEnemyOut(Enemy * enemy);

signals:
    void enemiesEnded();
    void enemyAttacksTheBase(const int &damage);

private:
    int id;         // Number of wave
    int interval;   // Intercal between this and next wave in seconds
    float dencity;  // Dencity of the wawe [0.1 .. 1]

    QList<Enemy*> groupOfEnemies;
    int currentEnemyId; // Id of the enemy that is processed
    int enemiesOutOfBattleNum; // Number of enemies that are already out of battle

    QGraphicsScene * scene;

    QTimer timerBetweenEnemies;
    unsigned int timerRemainingTimeOnPause;
};

#endif // WAVE_H
