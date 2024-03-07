#include "TowerMaster.h"

#include <QCoreApplication>
#include <QtMath>
#include <QDebug>

TowerMaster::TowerMaster() :
    scene(new QGraphicsScene(this))
{
    setRenderHint(QPainter::Antialiasing); // Set rendering hints for smoother zooming
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse); // Zoom centered around mouse cursor

    scene->setSceneRect(0, 0, 800, 600);

    setScene(scene);
    setFixedSize(800, 600); // TODO: Add settings. Size should be read from settings file
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // set cursor
    setMouseTracking(true);
    cursor = new Cursor(scene, scene->sceneRect());

    menuProcessor = new MenuProcessor(scene);

    connect(menuProcessor, &MenuProcessor::keyChoiseMade, this, &TowerMaster::handleMenuProcessor);
}

TowerMaster::~TowerMaster()
{
    if (menuProcessor) {
        delete menuProcessor;
    }

    if (gameProcessor) {
        delete gameProcessor;
    }

    delete cursor;

    delete scene;
}

void TowerMaster::mouseMoveEvent(QMouseEvent *event)
{
    cursor->setPos(mapToScene(event->pos()));
}

void TowerMaster::handleGameProcessor()
{
    menuProcessor = new MenuProcessor(scene);

    if (gameProcessor) {
        delete gameProcessor;
        gameProcessor = nullptr;
    }

    connect(menuProcessor, &MenuProcessor::keyChoiseMade, this, &TowerMaster::handleMenuProcessor);
    // Back to main menu => => process = Process::Menu && delete gameProcessor;
    // Quit delete gameProcessor; => quit
}

void TowerMaster::handleMenuProcessor()
{
    switch (menuProcessor->getChoice())
    {
        case MenuProcessor::StartGame:
        {
            // StartGame => Read Params => process = Process::Game && delete menuProcessor;
            gameProcessor = new GameProcessor(scene, menuProcessor->getLocationChoice(), cursor);

            if (menuProcessor) {
                delete menuProcessor;
                menuProcessor = nullptr;
            }

            connect(gameProcessor, &GameProcessor::mainMenuSignal, this, &TowerMaster::handleGameProcessor);

            break;
        }
        case MenuProcessor::Quit:
        {
            if (menuProcessor) {
                delete menuProcessor;
                menuProcessor = nullptr;
            }

            // Post a quit event to the application's event loop
            QCoreApplication::postEvent(QCoreApplication::instance(), new QEvent(QEvent::Quit));

            break;
        }
        default:
        {
            break;
        }
    }
}
