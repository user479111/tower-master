#include "MenuTextItem.h"

#include <QFont>
#include <QPen>

const int MenuTextItem::HIGHLIGHT_OFFSET = 5;

MenuTextItem::MenuTextItem(const QString &text,
                           int width,
                           int fontSize,
                           QPointF pos)
    : chosen(false),
      active(true)
{
    setPos(pos);

    setDefaultTextColor(Qt::black);
    setFont(QFont("Helvetica [Cronyx]", fontSize, QFont::Bold));

    backgroundRect = new QGraphicsRectItem(x(), y() + HIGHLIGHT_OFFSET, width, fontSize + HIGHLIGHT_OFFSET);
    backgroundRect->setBrush(Qt::transparent); // Set the fill color
    backgroundRect->setPen(QPen(Qt::transparent));

    setPlainText(text);
}

MenuTextItem::~MenuTextItem()
{
    delete backgroundRect;
}

void MenuTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void MenuTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseReleaseEvent(event);

    if (active) {
        setChosen(true);
        emit clicked(this);
    }
}

bool MenuTextItem::isChosen() const
{
    return chosen;
}

void MenuTextItem::setChosen(bool newChosen)
{
    chosen = newChosen;

    if (chosen) {

        setDefaultTextColor(Qt::white);
        backgroundRect->setBrush(QColor(153, 76, 0));

    } else {

        if (active) {
            setDefaultTextColor(Qt::black);
        } else {
            setDefaultTextColor(Qt::gray);
        }

        backgroundRect->setBrush(Qt::transparent);

    }
}

QGraphicsRectItem* MenuTextItem::getBackgroundRect() const
{
    return backgroundRect;
}

void MenuTextItem::setBackgroundRect(QGraphicsRectItem * newBackgroundRect)
{
    backgroundRect = newBackgroundRect;
}

bool MenuTextItem::getActive() const
{
    return active;
}

void MenuTextItem::setActive(bool newActive)
{
    active = newActive;

    if (active) {
        setDefaultTextColor(Qt::black);
    } else {
        setDefaultTextColor(Qt::gray);
    }
}
