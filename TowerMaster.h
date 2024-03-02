#ifndef TOWERMASTER_H
#define TOWERMASTER_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QSharedPointer>
#include <QMouseEvent>
#include <QObject>

#include "MenuProcessor.h"
#include "GameProcessor.h"
#include "Cursor.h"

class TowerMaster : public QGraphicsView
{
    Q_OBJECT
public:
    TowerMaster();
    ~TowerMaster();

    void mouseMoveEvent(QMouseEvent * event);

private slots:
    void handleMenuProcessor();
    void handleGameProcessor();

private:
    QSharedPointer<QGraphicsScene> scene;
    QSharedPointer<Cursor> cursor;
    QSharedPointer<MenuProcessor> menuProcessor;
    QSharedPointer<GameProcessor> gameProcessor;
};

#endif // TOWERMASTER_H
