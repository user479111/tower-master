#ifndef TOWERMASTER_H
#define TOWERMASTER_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QObject>

#include "MenuProcessor.h"
#include "GameProcessor.h"
#include "Preferences.h"
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
    QGraphicsScene * scene;
    Cursor * cursor;
    MenuProcessor * menuProcessor;
    GameProcessor * gameProcessor;
    Preferences * preferences;
};

#endif // TOWERMASTER_H
