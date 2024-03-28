#ifndef GAMEPROCESSOR_H
#define GAMEPROCESSOR_H

#include <QObject>
#include <QGraphicsScene>

#include "Cursor.h"
#include "Location.h"
#include "GameInterface.h"
#include "Battlefield.h"
#include "Preferences.h"

class GameProcessor : public QObject
{
    Q_OBJECT
public:
    explicit GameProcessor(Preferences * preferences,
                           QGraphicsScene * scene,
                           QString locationName,
                           Cursor * cursor,
                           QObject * parent = nullptr);

    ~GameProcessor();

private slots:
    void processRestartClick();
    void processMainMenuClick();

signals:
    void mainMenuSignal();

private:
    QGraphicsScene * scene;
    Cursor * cursor;
    Preferences * preferences;

    QString locationName;
    Location * location;
    Battlefield * battlefield;

    GameInterface * gameInterface;
};

#endif // GAMEPROCESSOR_H
