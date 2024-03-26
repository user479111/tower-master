#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <QGraphicsScene>
#include <QObject>
#include <QTimer>
#include <QList>

#include "Location.h"
#include "Cursor.h"
#include "Tower.h"
#include "Enemy.h"

#define MAX_SCALE 2

class Battlefield : public QObject
{
    Q_OBJECT
public:
    Battlefield(QGraphicsScene * scene,
                Cursor * cursor,
                Location * location);
    ~Battlefield();

    bool eventFilter(QObject *obj, QEvent *event);

    void addTower(Tower * newTower);

    const QList<Enemy*> &getGroupOfEnemies() const;

    Location* getLocation() const;

    void pause();
    void resume();

    int getEnemyReachedNumber() const;

    int getEnemyDamageGoal() const;

signals:
    void battlefieldScaled();
    void enemiesHaveBeenRun();

private slots:
    void processCursorMove();
    void startWaveMove();
    void startNextWave();

private:
    QGraphicsScene * scene;
    Cursor * cursor;
    Location * location;

    QList<Tower*> towers;

    QTimer timerBetweenWaves;
    unsigned int timerRemainingTimeOnPause;

    int currentWaveIndex;

    float minScale;

    bool scalingAllowed; // true by default

    int enemyReachedNumber; // Number of enemies that have reached the end of the route
};

#endif // BATTLEFIELD_H
