#ifndef GAMEPROCESSOR_H
#define GAMEPROCESSOR_H

#include <QObject>
#include <QGraphicsScene>

#include "Level.h"
#include "Cursor.h"
#include "GameInterface.h"
#include "Battlefield.h"
#include "Preferences.h"

class GameProcessor : public QObject
{
    Q_OBJECT
public:
    explicit GameProcessor(Preferences * preferences,
                           QGraphicsScene * scene,
                           int levelId,
                           Cursor * cursor,
                           QObject * parent = nullptr);

    ~GameProcessor();

    void processEscapePress();

private slots:
    void processRestartClick();
    void processMainMenuClick();

signals:
    void mainMenuSignal();

private:
    QGraphicsScene * scene;
    Cursor * cursor;
    Preferences * preferences;

    int levelId;
    Level * level;
    Battlefield * battlefield;

    GameInterface * gameInterface;
};

#endif // GAMEPROCESSOR_H
