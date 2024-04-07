#include "GameProcessor.h"

#include <QCursor>
#include <QDebug>

GameProcessor::GameProcessor(Preferences * preferences,
                             QGraphicsScene * scene,
                             int levelId,
                             Cursor * cursor,
                             QObject * parent)
    : QObject(parent),
      scene(scene),
      cursor(cursor),
      preferences(preferences),
      levelId(levelId),
      level(new Level(preferences, scene, levelId)),
      battlefield(new Battlefield(scene, cursor, level)),
      gameInterface(new GameInterface(preferences, scene, cursor, battlefield))
{
    scene->setBackgroundBrush(QBrush(Qt::black));

    scene->installEventFilter(this);

    connect(gameInterface, &GameInterface::restart, this, &GameProcessor::processRestartClick);
    connect(gameInterface, &GameInterface::nextLevel, this, &GameProcessor::processNextLevelSignal);
    connect(gameInterface, &GameInterface::mainMenuSignal, this, &GameProcessor::processMainMenuClick);
}

GameProcessor::~GameProcessor()
{
    if (level) {
        delete level;
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

    if (level) {
        delete level;
    }
    level = new Level(preferences, scene, levelId);

    if (battlefield) {
        delete battlefield;
    }
    battlefield = new Battlefield(scene, cursor, level);

    if (gameInterface) {
        delete gameInterface;
    }
    gameInterface = new GameInterface(preferences, scene, cursor, battlefield);

    connect(gameInterface, &GameInterface::restart, this, &GameProcessor::processRestartClick);
    connect(gameInterface, &GameInterface::nextLevel, this, &GameProcessor::processNextLevelSignal);
    connect(gameInterface, &GameInterface::mainMenuSignal, this, &GameProcessor::processMainMenuClick);
}

void GameProcessor::processMainMenuClick()
{
    emit mainMenuSignal();
}

void GameProcessor::processNextLevelSignal()
{
    levelId++;

    processRestartClick();
}
