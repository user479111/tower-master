#include "Minimap.h"
#include <QDebug>

Minimap::Minimap(QSharedPointer<QGraphicsScene> scene,
                 const QPixmap &pixmap,
                 QSharedPointer<QGraphicsPixmapItem> parent) :
    QGraphicsPixmapItem(pixmap , parent.get()),
    scene(scene),
    clickPosition(0, 0),
    mapScale(1)
{
}

void Minimap::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    clickPosition = QPointF(event->pos().x() * 0.1 /* move scale to xml */, event->pos().y() * 0.1 /* move scale to xml */);

    // Calculate the point on the map that represents the click point on the minimap
    // X
    if (event->pos().x() * mapScale <= scene->sceneRect().width() / 2) {
        calculatedScenePosition.setX(scene->sceneRect().width() / 2);
    }
    else if (event->pos().x() * mapScale >= boundingRect().width() * mapScale - scene->sceneRect().width() / 2) {
        calculatedScenePosition.setX(boundingRect().width() * mapScale - scene->sceneRect().width() / 2);
    } else {
        calculatedScenePosition.setX(event->pos().x() * mapScale);
    }
    // Y
    if (event->pos().y() * mapScale <= scene->sceneRect().height() / 2) {
        calculatedScenePosition.setY(scene->sceneRect().height() / 2);
    }
    else if (event->pos().y() * mapScale >= boundingRect().height() * mapScale - scene->sceneRect().height() / 2) {
        calculatedScenePosition.setY(boundingRect().height() * mapScale - scene->sceneRect().height() / 2);
    } else {
        calculatedScenePosition.setY(event->pos().y() * mapScale);
    }

    emit mousePressed();
}

QPointF Minimap::getClickPosition() const
{
    return clickPosition;
}

float Minimap::getMapScale() const
{
    return mapScale;
}

void Minimap::setMapScale(float newMapScale)
{
    mapScale = newMapScale;
}

QPointF Minimap::getCalculatedScenePosition() const
{
    return calculatedScenePosition;
}
