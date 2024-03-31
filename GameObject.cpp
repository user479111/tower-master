#include "GameObject.h"

#include <QPen>
#include <QDebug>

int GameObject::gameObjectCount = 0;
int GameObject::highlightedObjectId = -1;

GameObject::GameObject(QObject *parent)
    : QObject{parent},
      highlight(new QGraphicsEllipseItem),
      suitableForHighlighting(true)
{
    id = gameObjectCount;
    gameObjectCount++;

    highlight->setPen(QPen(Qt::transparent));
}

GameObject::~GameObject()
{
    delete highlight;
}

void GameObject::setPixmap(const QPixmap &pixmap)
{
    QGraphicsPixmapItem::setPixmap(pixmap);

    float diameter = QLineF(QGraphicsPixmapItem::boundingRect().bottomLeft(),
                            QGraphicsPixmapItem::boundingRect().topRight()).length();

    highlight->setRect(0, 0, diameter, diameter);
}

void GameObject::setPos(qreal x, qreal y)
{
    setPos(QPointF(x, y));
}

void GameObject::setPos(const QPointF &pos)
{
    QGraphicsPixmapItem::setPos(pos);

    highlight->setPos(QGraphicsPixmapItem::pos().x() +
                      QGraphicsPixmapItem::boundingRect().width() / 2 * QGraphicsPixmapItem::scale() -
                      highlight->boundingRect().width() / 2 * QGraphicsPixmapItem::scale(),
                      QGraphicsPixmapItem::pos().y() +
                      QGraphicsPixmapItem::boundingRect().height() / 2 * QGraphicsPixmapItem::scale() -
                      highlight->boundingRect().height() / 2 * QGraphicsPixmapItem::scale());
}

void GameObject::setScale(qreal scale)
{
    QGraphicsPixmapItem::setScale(scale);
    highlight->setScale(scale);
}

void GameObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!suitableForHighlighting) {
        return;
    }

    highlightedObjectId = id;

    emit clicked(this);
}

void GameObject::setHighlighting()
{
    if (id == highlightedObjectId) {
        highlight->setPen(QPen(Qt::red));
    } else {
        highlight->setPen(QPen(Qt::transparent));
    }
}

void GameObject::show(QGraphicsScene *scene)
{
    scene->addItem(this);
    scene->addItem(highlight);
}

void GameObject::hide(QGraphicsScene *scene)
{
    scene->removeItem(this);
    scene->removeItem(highlight);
}

void GameObject::setSuitableForHighlighting(bool value)
{
    suitableForHighlighting = value;
}

int GameObject::getId() const
{
    return id;
}

void GameObject::resetHighlightedObjectId()
{
    highlightedObjectId = -1;
}
