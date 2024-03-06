#include "GameProcessor.h"

#include <QDebug>

GameProcessor::GameProcessor(QGraphicsScene * scene,
                             QString locationName,
                             Cursor * cursor,
                             QObject * parent)
    : QObject(parent),
      scene(scene),
      cursor(cursor),
      location(new Location(scene, locationName)),
      battlefield(new Battlefield(scene, cursor, location)),
      gameInterface(new GameInterface(scene, cursor, battlefield))
{
    scene->setBackgroundBrush(QBrush(Qt::black));

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

void GameProcessor::processMainMenuClick()
{
    emit mainMenuSignal();
}
