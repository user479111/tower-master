#ifndef GAMEPROCESSOR_H
#define GAMEPROCESSOR_H

#include <QObject>
#include <QGraphicsScene>
#include <QMouseEvent>

#include "Cursor.h"
#include "Location.h"
#include "GameInterface.h"
#include "Battlefield.h"

class GameProcessor : public QObject
{
    Q_OBJECT
public:
    explicit GameProcessor(QGraphicsScene * scene,
                           QString locationName,
                           Cursor * cursor,
                           QObject * parent = nullptr);

    ~GameProcessor();

private slots:
    void processMainMenuClick();

signals:
    void mainMenuSignal();

private:
    QGraphicsScene * scene;
    Cursor * cursor;

    Location * location;
    Battlefield * battlefield;

    GameInterface * gameInterface;
};

#endif // GAMEPROCESSOR_H
