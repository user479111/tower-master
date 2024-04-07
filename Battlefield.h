#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <QGraphicsScene>
#include <QObject>
#include <QTimer>
#include <QList>

#include "Level.h"
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
                Level * level);
    ~Battlefield();

    bool eventFilter(QObject *obj, QEvent *event);

    void addTower(Tower * newTower);

    const QList<Enemy*> &getGroupOfEnemies() const;

    Location* getLocation() const;

    void pause();
    void resume();

    int getEnemyReachedNumber() const;

    int getEnemyDamageGoal() const;

    void clearTowers();

    bool getPaused() const;

    const QList<Tower *> &getTowers() const;

    void updateGameObjectsHighlighting();

    Level *getLevel() const;

    bool getLevelCompleted() const;
    void setLevelCompleted(bool newLevelCompleted);

signals:
    void battlefieldScaled();
    void enemiesHaveBeenRun();
    void enemyCausedDamage();
    void gameOver();
    void victory();
    void enemyClicked(const GameObject * object);
    void highlightedEnemyOut();
    void highlightedEnemyUpdate(const Enemy * enemy);

private slots:
    void processCursorMove();
    void startWaveMove();
    void startNextWave();
    void processEnemyAttack(const int &damage);
    void processEnemyClicked(const GameObject * object);
    void processHighlightedEnemyOut();
    void processHighlightedEnemyUpdate(const Enemy * enemy);

private:
    QGraphicsScene * scene;
    Cursor * cursor;
    Level * level;

    QList<Tower*> towers;

    QTimer timerBetweenWaves;
    unsigned int timerRemainingTimeOnPause;

    int currentWaveIndex;

    float minScale;

    bool paused;
    bool levelCompleted;

    int enemyReachedNumber; // Number of enemies that have reached the end of the route
};

#endif // BATTLEFIELD_H
