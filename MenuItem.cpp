#include "MenuItem.h"

#include <QDebug>
#include <QFont>

MenuItem::MenuItem(QGraphicsItem * parent) :
    QGraphicsPixmapItem(parent),
    title(""),
    chosen(false),
    staticSize(false),
    text(new QGraphicsTextItem)
{
}

MenuItem::~MenuItem()
{
    delete text;
}

void MenuItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (staticSize) { return; }

    setTransform(QTransform().
                 translate(pixmap().width()/2 * scale(), pixmap().height()/2 * scale()).
                 scale(1.5, 1.5).
                 translate(-pixmap().width()/2 * scale(), -pixmap().height()/2 * scale()));

    text->setTransform(QTransform().
                        translate(text->boundingRect().width()/2 * scale(), text->boundingRect().height()/2 * scale()).
                        scale(1.5, 1.5).
                        translate(-text->boundingRect().width()/2 * scale(), -text->boundingRect().height()/2 * scale()));
}

void MenuItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!staticSize) {
        setTransform(QTransform().
                     translate(pixmap().width()/2 * scale(), pixmap().height()/2 * scale()).
                     scale(1, 1).
                     translate(-pixmap().width()/2 * scale(), -pixmap().height()/2 * scale()));

        text->setTransform(QTransform().
                            translate(text->boundingRect().width()/2 * scale(), text->boundingRect().height()/2 * scale()).
                            scale(1, 1).
                            translate(-text->boundingRect().width()/2 * scale(), -text->boundingRect().height()/2 * scale()));
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

void MenuItem::setScale(qreal scale)
{
    QGraphicsPixmapItem::setScale(scale);

    text->setScale(scale);
}

void MenuItem::setPos(const QPointF &pos)
{
    QGraphicsPixmapItem::setPos(pos);

    text->setPos(pos.x() + pixmap().width() * scale() / 2 - text->boundingRect().width() * scale() / 2,
                 pos.y() + pixmap().height() * scale() / 2 - text->boundingRect().height() * scale() / 2);
}

void MenuItem::setZValue(qreal z)
{
    QGraphicsPixmapItem::setZValue(z);
    text->setZValue(z);
}

void MenuItem::setText(const QString &text)
{
    this->text->setDefaultTextColor(Qt::black);
    this->text->setFont(QFont("Helvetica [Cronyx]", 14 /* TODO: Take from Preferences */ , QFont::Bold));
    this->text->setPlainText(text);

    this->text->setPos(pos().x() + pixmap().width() * scale() / 2 - this->text->boundingRect().width() * scale() / 2,
                      pos().y() + pixmap().height() * scale() / 2 - this->text->boundingRect().height() * scale() / 2);
}

void MenuItem::show(QGraphicsScene *scene)
{
    scene->addItem(this);
    scene->addItem(text);
}

void MenuItem::hide(QGraphicsScene *scene)
{
    scene->removeItem(this);
    scene->removeItem(text);
}
