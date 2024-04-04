#include "Cursor.h"

#include <QDebug>

// Define the margin (distance from edge to start moving the view)
const int Cursor::MARGIN_CLOSE = 20;
const int Cursor::FAST_SCROLL = 20;

const int Cursor::MARGIN_FAR = 40;
const int Cursor::SLOW_SCROLL = 10;

Cursor::Cursor(QGraphicsScene * scene) :
    scene(scene),
    scrollAreaRect(scene->sceneRect()),
    buildMode(false),
    buildIsPossible(false),
    scrollingAllowed(false)
{
    setPixmap(QPixmap(":/Data/Data/Cursor/Cursor.png")); // TODO: move hardcoded paths somewhere

    // Set the ZValue of the pixmap item to a very high value
    // This ensures it will always be on top of other items
    setZValue(std::numeric_limits<qreal>::max());

    scene->addItem(this);

    // Create a timer for continuous scrolling
    connect(&scrollTimer, &QTimer::timeout, this, &Cursor::checkScrollArea);
    scrollTimer.start(50); // Adjust the interval as needed (e.g., 50 milliseconds)
}

void Cursor::checkScrollArea()
{
    if (!scrollingAllowed) {
        return;
    }

    // Get the current cursor rectangle
    QRectF currentRect = mapRectToScene(boundingRect());

    // Get the current scene rectangle
    QRectF currentSceneRect = scene->sceneRect();

    // Keep the current scene rectangle for comparison
    QRectF previousSceneRect = currentSceneRect;


    // Check if the mouse is close to the edge of the view
    if (((currentRect.x() < currentSceneRect.x() + MARGIN_CLOSE) ||
            ((currentRect.center().x() < currentSceneRect.x() + MARGIN_CLOSE) && buildMode)) &&
            (currentSceneRect.left() > scrollAreaRect.left())) // prevent scroll out of the left border
    {

        // Move view to the left
        currentSceneRect.translate(-FAST_SCROLL, 0);
        // Move cursor correspondingly
        QGraphicsPixmapItem::setPos(QPointF(currentRect.left() - FAST_SCROLL, currentRect.top()));

    } else if (((currentRect.x() < currentSceneRect.x() + MARGIN_FAR) ||
               ((currentRect.center().x() < currentSceneRect.x() + MARGIN_FAR) && buildMode)) &&
               (currentSceneRect.left() > scrollAreaRect.left())) // prevent scroll out of the left border
    {

        // Move view to the left
        currentSceneRect.translate(-SLOW_SCROLL, 0);
        // Move cursor correspondingly
        QGraphicsPixmapItem::setPos(QPointF(currentRect.left() - SLOW_SCROLL, currentRect.top()));

    } else if (((currentRect.x() + currentRect.width() / 2 > currentSceneRect.right() - MARGIN_CLOSE) ||
                ((currentRect.center().x() > currentSceneRect.right() - MARGIN_CLOSE) && buildMode)) &&
               (currentSceneRect.right() < scrollAreaRect.right())) // prevent scroll out of the right border
    {

        // Move view to the right
        currentSceneRect.translate(FAST_SCROLL, 0);
        // Move cursor correspondingly
        QGraphicsPixmapItem::setPos(QPointF(currentRect.left() + FAST_SCROLL, currentRect.top()));

    } else if (((currentRect.x() + currentRect.width() / 2 > currentSceneRect.right() - MARGIN_FAR) ||
              ((currentRect.center().x() > currentSceneRect.right() - MARGIN_FAR) && buildMode)) &&
            (currentSceneRect.right() < scrollAreaRect.right())) // prevent scroll out of the right border
    {

       // Move view to the right
       currentSceneRect.translate(SLOW_SCROLL, 0);
       // Move cursor correspondingly
       QGraphicsPixmapItem::setPos(QPointF(currentRect.left() + SLOW_SCROLL, currentRect.top()));

    } else if (((currentRect.top() < currentSceneRect.top() + MARGIN_CLOSE) ||
                ((currentRect.center().y() < currentSceneRect.top() + MARGIN_CLOSE) && buildMode)) &&
               (currentSceneRect.top() > scrollAreaRect.top())) // prevent scroll out of the top border
    {

        // Move view upwards
        currentSceneRect.translate(0, -FAST_SCROLL);
        // Move cursor correspondingly
        QGraphicsPixmapItem::setPos(QPointF(currentRect.left(), currentRect.top() - FAST_SCROLL));

    } else if (((currentRect.top() < currentSceneRect.top() + MARGIN_FAR) ||
                   ((currentRect.center().y() < currentSceneRect.top() + MARGIN_FAR) && buildMode)) &&
                   (currentSceneRect.top() > scrollAreaRect.top())) // prevent scroll out of the top border
    {

        // Move view upwards
        currentSceneRect.translate(0, -SLOW_SCROLL);
        // Move cursor correspondingly
        QGraphicsPixmapItem::setPos(QPointF(currentRect.left(), currentRect.top() - SLOW_SCROLL));

    } else if (((currentRect.top() + currentRect.height() / 2 > currentSceneRect.bottom() - MARGIN_CLOSE) ||
                ((currentRect.center().y() > currentSceneRect.bottom() - MARGIN_CLOSE) && buildMode)) &&
               (currentSceneRect.bottom() < scrollAreaRect.bottom())) // prevent scroll out of the bottom border
    {

        // Move view downwards
        currentSceneRect.translate(0, FAST_SCROLL);
        // Move cursor correspondingly
        QGraphicsPixmapItem::setPos(QPointF(currentRect.left(), currentRect.top() + FAST_SCROLL));

    } else if (((currentRect.top() + currentRect.height() / 2 > currentSceneRect.bottom() - MARGIN_FAR) ||
                   ((currentRect.center().y() > currentSceneRect.bottom() - MARGIN_FAR) && buildMode)) &&
               (currentSceneRect.bottom() < scrollAreaRect.bottom())) // prevent scroll out of the bottom border
    {

        // Move view downwards
        currentSceneRect.translate(0, SLOW_SCROLL);
        // Move cursor correspondingly
        QGraphicsPixmapItem::setPos(QPointF(currentRect.left(), currentRect.top() + SLOW_SCROLL));

    }

    if (previousSceneRect != currentSceneRect) {
        // Set the updated scene rectangle to adjust the view
        scene->setSceneRect(currentSceneRect);
        emit areaScrolled();
    }
}

bool Cursor::getScrollingAllowed() const
{
    return scrollingAllowed;
}

void Cursor::setScrollingAllowed(bool newScrollingAllowed)
{
    scrollingAllowed = newScrollingAllowed;
}

bool Cursor::getBuildIsPossible() const
{
    return buildIsPossible;
}

void Cursor::processEscapePress()
{
    if (buildMode) {
        // Escape from build mode
        setBuildMode(false);
    }
}

bool Cursor::getBuildMode() const
{
    return buildMode;
}

void Cursor::setBuildMode(const bool newBuildMode, const QString &towerType, const qreal scale)
{
    buildMode = newBuildMode;

    if (newBuildMode) {

        if (towerType.isEmpty()) {
            return;
        }

        this->towerType = towerType;

        setBuildImage(false);
        QGraphicsPixmapItem::setPos(pos().x() - pixmap().width()/2, pos().y() - pixmap().height()/2);

    } else {

        auto previousPosition = QPointF(pos().x() + pixmap().width()/2, pos().y() + pixmap().height()/2);
        setPixmap(QString(":/Data/Data/Cursor/Cursor.png"));
        QGraphicsPixmapItem::setPos(previousPosition);

    }

    setScale(scale);
}

void Cursor::setPos(const QPointF &pos)
{
    if (!buildMode) {
        QGraphicsPixmapItem::setPos(pos);
    } else {
        QGraphicsPixmapItem::setPos(pos.x() - pixmap().width()/2, pos.y() - pixmap().height()/2);
    }

    emit cursorMoved();
}

void Cursor::setScale(qreal scale)
{
    if (!buildMode) {
        setTransform(QTransform().scale(scale, scale));
    } else {
        setTransform(QTransform().translate(pixmap().width()/2, pixmap().height()/2).
            scale(scale, scale).
            translate(-pixmap().width()/2, -pixmap().height()/2));
    }
}

void Cursor::setBuildImage(bool buildIsPossible)
{
    if (!buildMode) {
        return;
    }

    this->buildIsPossible = buildIsPossible;

    if (buildIsPossible) {
        setPixmap(QString(":/Data/Data/Towers/" + towerType + "/TowerAllowBuilding.png"));
    } else {
        setPixmap(QString(":/Data/Data/Towers/" + towerType + "/TowerProhibitBuilding.png"));
    }
}

const QRectF &Cursor::getScrollAreaRect() const
{
    return scrollAreaRect;
}

void Cursor::setScrollAreaRect(const QRectF &newScrollAreaRect)
{
    scrollAreaRect = newScrollAreaRect;
}

// Note: if "real" cursor is not located on the pixmap the mousePressEvent
//       procedure won't be called
void Cursor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPixmapItem::mousePressEvent(event);
    emit mousePressed();
}
