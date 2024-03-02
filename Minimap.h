#ifndef MINIMAP_H
#define MINIMAP_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QSharedPointer>
#include <QPointF>
#include <QObject>

class Minimap : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Minimap(QSharedPointer<QGraphicsScene> scene,
            const QPixmap &pixmap,
            QSharedPointer<QGraphicsPixmapItem> parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    QPointF getClickPosition() const;

    float getMapScale() const;
    void setMapScale(float newMapScale);

    QPointF getCalculatedScenePosition() const;

signals:
    void mousePressed();

private:
    QSharedPointer<QGraphicsScene> scene;
    QPointF clickPosition;
    QPointF calculatedScenePosition;
    float mapScale;
};

#endif // MINIMAP_H
