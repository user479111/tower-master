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

    setScene(scene.get());
    setFixedSize(800, 600); // TODO: Add settings. Size should be read from settings file
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // set cursor
    setMouseTracking(true);
    cursor = QSharedPointer<Cursor>(new Cursor(scene, scene->sceneRect()));

    menuProcessor = QSharedPointer<MenuProcessor>(new MenuProcessor(scene));

    connect(menuProcessor.get(), &MenuProcessor::keyChoiseMade, this, &TowerMaster::handleMenuProcessor);
}

TowerMaster::~TowerMaster()
{
}

void TowerMaster::mouseMoveEvent(QMouseEvent *event)
{
    cursor->setPos(mapToScene(event->pos()));
}

void TowerMaster::handleGameProcessor()
{
    qDebug() << "TowerMaster::handleGameProcessor";
    menuProcessor = QSharedPointer<MenuProcessor>(new MenuProcessor(scene));

    qDebug() << "TowerMaster::handleGameProcessor 1";
    if (gameProcessor) { gameProcessor.reset(); }
    qDebug() << "TowerMaster::handleGameProcessor 2";
    connect(menuProcessor.get(), &MenuProcessor::keyChoiseMade, this, &TowerMaster::handleMenuProcessor);
    // Back to main menu => => process = Process::Menu && delete gameProcessor;
    // Quit delete gameProcessor; => quit
}

void TowerMaster::handleMenuProcessor()
{
    qDebug() << "TowerMaster::handleMenuProcessor 0";
    switch (menuProcessor->getChoice())
    {
        case MenuProcessor::StartGame:
        {
            // StartGame => Read Params => process = Process::Game && delete menuProcessor;
            qDebug() << "TowerMaster::handleMenuProcessor";
            gameProcessor = QSharedPointer<GameProcessor>(new GameProcessor(scene, menuProcessor->getLocationChoice(), cursor));

            if (menuProcessor) { menuProcessor.reset(); }

            connect(gameProcessor.get(), &GameProcessor::mainMenuSignal, this, &TowerMaster::handleGameProcessor);

            break;
        }
        case MenuProcessor::Quit:
        {
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
