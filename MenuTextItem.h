#ifndef MENUTEXTITEM_H
#define MENUTEXTITEM_H

#include <QGraphicsTextItem>

class MenuTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    MenuTextItem(const QString &text,
                 int width,
                 int fontSize,
                 QPointF pos);
    ~MenuTextItem();

    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    bool isChosen() const;
    void setChosen(bool newChosen);

    QGraphicsRectItem * getBackgroundRect() const;
    void setBackgroundRect(QGraphicsRectItem * newBackgroundRect);

    bool getActive() const;
    void setActive(bool newActive);

signals:
    void clicked(MenuTextItem * Choice);

private:
    static const int HIGHLIGHT_OFFSET;
    QGraphicsRectItem * backgroundRect;
    bool chosen;
    bool active;
};

#endif // MENUTEXTITEM_H
