#include "GameProcessor.h"

#include <QDebug>

GameProcessor::GameProcessor(QSharedPointer<QGraphicsScene> scene,
                             QString locationName,
                             QSharedPointer<Cursor> cursor,
                             QSharedPointer<QObject> parent)
    : QObject(parent.get()),
      scene(scene),
      cursor(cursor),
      location(new Location(locationName, static_cast<QSharedPointer<QGraphicsScene>>(scene))),
      battlefield(new Battlefield(scene, cursor, location)),
      gameInterface(new GameInterface(scene, cursor, battlefield))
{
    qDebug() << "GameProcessor::GameProcessor";
    scene->setBackgroundBrush(QBrush(Qt::black));

    connect(gameInterface.get(), &GameInterface::mainMenuSignal, this, &GameProcessor::processMainMenuClick);
}

GameProcessor::~GameProcessor()
{
    qDebug() << "GameProcessor::~GameProcessor";
}

void GameProcessor::processMainMenuClick()
{
    emit mainMenuSignal();
}
