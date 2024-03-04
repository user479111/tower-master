#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <QGraphicsScene>
#include <QSharedPointer>
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
    Battlefield(QSharedPointer<QGraphicsScene> scene,
                QSharedPointer<Cursor> cursor,
                QSharedPointer<Location> location);
    ~Battlefield();

    bool eventFilter(QObject *obj, QEvent *event);

    QSharedPointer<Location> getLocation() const;

    void addTower(QSharedPointer<Tower> newTower);

    const QList<Enemy*> &getGroupOfEnemies() const;

signals:
    void battlefieldScaled();
    void enemiesHaveBeenRun();

private slots:
    void processCursorMove();
    void startWaveMove();
    void startNextWave();

private:
    QSharedPointer<QGraphicsScene> scene;
    QSharedPointer<Cursor> cursor;
    QSharedPointer<Location> location;

    QList<QSharedPointer<Tower>> towers;

    QTimer timerBetweenWaves;

    int currentWaveIndex;

    float minScale;
};

#endif // BATTLEFIELD_H
