#include "TowerMaster.h"

#include <QCoreApplication>
#include <QApplication>
#include <QScreen>
#include <QtMath>
#include <QDebug>

TowerMaster::TowerMaster() :
    scene(new QGraphicsScene(this)),
    preferences(new Preferences)
{    
    setRenderHint(QPainter::Antialiasing); // Set rendering hints for smoother zooming
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    applyScreenPreferences();

    setScene(scene);

    // set cursor
    setMouseTracking(true);
    cursor = new Cursor(scene);

    menuProcessor = new MenuProcessor(preferences, scene);

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

void TowerMaster::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

    if (event->button() == Qt::RightButton) {
        cursor->setScrollingAllowed(true);
    }
}

void TowerMaster::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);

    if (event->button() == Qt::RightButton) {
        cursor->setScrollingAllowed(false);
    }
}

void TowerMaster::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        if (gameProcessor) {
            gameProcessor->processEscapePress();
        }
    }
}

void TowerMaster::applyScreenPreferences()
{
    setWindowState(preferences->getFullscreen() ? Qt::WindowFullScreen : Qt::WindowNoState);

    // Calculate the view size depending on the screen mode
    QRect screenGeometry;
    if (preferences->getFullscreen()) {
        // Get the available screens
        QList<QScreen*> screens = QGuiApplication::screens();
        if (!screens.isEmpty()) {
            // Get the geometry of the first screen
            screenGeometry = screens.first()->geometry();

            // reset geometry for view to make the application full sized
            setGeometry(screenGeometry);
        }

        showFullScreen();

    } else {
        screenGeometry = QRect(0, 0, preferences->getResolution().width, preferences->getResolution().height);
    }

    setFixedSize(screenGeometry.width(), screenGeometry.height());

    // Ensure that the draw will start wrom the top left corner
    setAlignment(Qt::AlignLeft | Qt::AlignTop);

    scene->setSceneRect(0, 0, preferences->getResolution().width, preferences->getResolution().height);

    // Scale and translate the scene to fit within its viewport

    qreal scaleX = width() / (float) preferences->getResolution().width;
    qreal scaleY = height() / (float) preferences->getResolution().height;

    // Set the transformation matrix to scale the scene to fit within the view
    resetTransform();
    scale(scaleX, scaleY);
}

void TowerMaster::handleGameProcessor()
{
    menuProcessor = new MenuProcessor(preferences, scene);

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
            gameProcessor = new GameProcessor(preferences, scene, menuProcessor->getLocationChoice(), cursor);

            if (menuProcessor) {
                delete menuProcessor;
                menuProcessor = nullptr;
            }

            connect(gameProcessor, &GameProcessor::mainMenuSignal, this, &TowerMaster::handleGameProcessor);

            break;
        }
        case MenuProcessor::ApplySettings:
        {
            scene->setSceneRect(0, 0, preferences->getResolution().width, preferences->getResolution().height);

            applyScreenPreferences();

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
