#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "QGraphicsSceneMouseEvent"
#include "QGraphicsEllipseItem"
#include "QGraphicsPixmapItem"

#include <QGraphicsScene>
#include <QPixmap>
#include <QPointF>
#include <QObject>


class GameObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    GameObject(QObject *parent = nullptr);
    ~GameObject();

    void setPixmap(const QPixmap &pixmap);

    void setPos(qreal x, qreal y);
    void setPos(const QPointF &pos);

    void setScale(qreal scale);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    void setHighlighting();

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);

    void setSuitableForHighlighting(bool value);

    int getId() const;

    static void resetHighlightedObjectId();

signals:
    void clicked(const GameObject * object);

protected:
    static int highlightedObjectId;
    static int gameObjectCount;
private:

    QGraphicsEllipseItem * highlight;

    int id;

    // Some game objects (like Bullets) are not suitable for highlighting
    bool suitableForHighlighting;
};

#endif // GAMEOBJECT_H
