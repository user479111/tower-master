#include "MenuItem.h"
#include <QDebug>
MenuItem::MenuItem(QGraphicsItem * parent) :
    QGraphicsPixmapItem(parent),
    title(""),
    chosen(false),
    staticSize(false)
{
}

void MenuItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (staticSize) { return; }

    setTransform(QTransform().
                 translate(pixmap().width()/2, pixmap().height()/2).
                 scale(1.5, 1.5).
                 translate(-pixmap().width()/2, -pixmap().height()/2));
}

void MenuItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!staticSize) {
        setTransform(QTransform().
                     translate(pixmap().width()/2, pixmap().height()/2).
                     scale(1, 1).
                     translate(-pixmap().width()/2, -pixmap().height()/2));
    }

    setChosen(true);

    emit clicked();
}

const QString &MenuItem::getTitle() const
{
    return title;
}

void MenuItem::setTitle(const QString &newTitle)
{
    title = newTitle;
}

bool MenuItem::isChosen() const
{
    return chosen;
}

void MenuItem::setChosen(bool newChosen)
{
    chosen = newChosen;
}

bool MenuItem::getStaticSize() const
{
    return staticSize;
}

void MenuItem::setStaticSize(bool newStaticSize)
{
    staticSize = newStaticSize;
}
