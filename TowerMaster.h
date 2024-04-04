#ifndef TOWERMASTER_H
#define TOWERMASTER_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QKeyEvent>
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
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

    void keyPressEvent(QKeyEvent *event);

private:
    void applyScreenPreferences();

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
