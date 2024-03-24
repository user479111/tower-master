#include "Cursor.h"

#include <QDebug>

Cursor::Cursor(QGraphicsScene * scene,
               const QRectF &inScrollAreaRect) :
    scene(scene),
    scrollAreaRect(inScrollAreaRect),
    buildMode(false),
    buildIsPossible(false)
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

// TODO: margin and 10s should be defined in a different way
void Cursor::checkScrollArea()
{
    // Get the current cursor rectangle
    QRectF currentRect = mapRectToScene(boundingRect());

    // Get the current scene rectangle
    QRectF currentSceneRect = scene->sceneRect();

    // Keep the current scene rectangle for comparison
    QRectF previousSceneRect = currentSceneRect;

    // Define the margin (distance from edge to start moving the view)
    int margin = 5;

    // Check if the mouse is close to the edge of the view
    if ((currentSceneRect.left() > scrollAreaRect.left()) &&            // prevent scroll out of the left border
            ((currentRect.x() < currentSceneRect.x() + margin) ||
            ((currentRect.center().x() < currentSceneRect.x() + margin) && buildMode))) {

        currentSceneRect.translate(-10, 0); // Move view to the left
        QGraphicsPixmapItem::setPos(QPointF(currentRect.left() - 10, currentRect.top()));

    } else if ((currentSceneRect.right() < scrollAreaRect.right()) &&   // prevent scroll out of the right border
               ((currentRect.x() > currentSceneRect.right() - margin) ||
               ((currentRect.center().x() > currentSceneRect.right() - margin) && buildMode))) {

        currentSceneRect.translate(10, 0); // Move view to the right
        QGraphicsPixmapItem::setPos(QPointF(currentRect.left() + 10, currentRect.top()));

    }

    if ((currentSceneRect.top() > scrollAreaRect.top()) &&              // prevent scroll out of the top border
            ((currentRect.top() < currentSceneRect.top() + margin) ||
            ((currentRect.center().y() < currentSceneRect.top() + margin) && buildMode))) {

        currentSceneRect.translate(0, -10); // Move view upwards
        QGraphicsPixmapItem::setPos(QPointF(currentRect.left(), currentRect.top() - 10));

    } else if ((currentSceneRect.bottom() < scrollAreaRect.bottom()) && // prevent scroll out of the bottom border
               ((currentRect.top() > currentSceneRect.bottom() - margin) ||
                ((currentRect.center().y() > currentSceneRect.bottom() - margin) && buildMode))) {

        currentSceneRect.translate(0, 10); // Move view downwards
        QGraphicsPixmapItem::setPos(QPointF(currentRect.left(), currentRect.top() + 10));

    }

    if (previousSceneRect != currentSceneRect) {
        // Set the updated scene rectangle to adjust the view
        scene->setSceneRect(currentSceneRect);
        emit areaScrolled();
    }
}

bool Cursor::getBuildIsPossible() const
{
    return buildIsPossible;
}

void Cursor::checkEscape(QKeyEvent *event)
{
    if (buildMode && event->key() == Qt::Key_Escape) {
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
    emit mousePressed();
}
