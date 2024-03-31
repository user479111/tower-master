#include "GameProcessor.h"

#include <QCursor>
#include <QDebug>

GameProcessor::GameProcessor(Preferences * preferences,
                             QGraphicsScene * scene,
                             QString locationName,
                             Cursor * cursor,
                             QObject * parent)
    : QObject(parent),
      scene(scene),
      cursor(cursor),
      preferences(preferences),
      locationName(locationName),
      location(new Location(scene, locationName)),
      battlefield(new Battlefield(scene, cursor, location)),
      gameInterface(new GameInterface(preferences, scene, cursor, battlefield))
{
    scene->setBackgroundBrush(QBrush(Qt::black));

    scene->installEventFilter(this);

    connect(gameInterface, &GameInterface::restart, this, &GameProcessor::processRestartClick);
    connect(gameInterface, &GameInterface::mainMenuSignal, this, &GameProcessor::processMainMenuClick);
}

GameProcessor::~GameProcessor()
{
    if (location) {
        delete location;
    }

    if (battlefield) {
        delete battlefield;
    }

    if (gameInterface) {
        delete gameInterface;
    }
}

void GameProcessor::processEscapePress()
{
    gameInterface->processEscapePress();
}

void GameProcessor::processRestartClick()
{
    scene->setSceneRect(QRectF(0, 0, scene->sceneRect().width(), scene->sceneRect().height()));

    cursor->setPos(scene->views().first()->mapFromGlobal(QCursor::pos()));

    if (location) {
        delete location;
    }
    location = new Location(scene, locationName);

    if (battlefield) {
        delete battlefield;
    }
    battlefield = new Battlefield(scene, cursor, location);

    if (gameInterface) {
        delete gameInterface;
    }
    gameInterface = new GameInterface(preferences, scene, cursor, battlefield);

    connect(gameInterface, &GameInterface::restart, this, &GameProcessor::processRestartClick);
    connect(gameInterface, &GameInterface::mainMenuSignal, this, &GameProcessor::processMainMenuClick);
}

void GameProcessor::processMainMenuClick()
{
    emit mainMenuSignal();
}
