#ifndef MenuItem_H
#define MenuItem_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QObject>


class MenuItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    enum Align { Global, // to Scene coordinates
                 TopLeft, // to top-left of the menu
                 Center}; // to the center of the menu

    MenuItem(QGraphicsItem * parent = 0);
    ~MenuItem();

    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    const QString &getTitle() const;
    void setTitle(const QString &newTitle);

    bool isChosen() const;
    void setChosen(bool newChosen);

    bool getStaticSize() const;
    void setStaticSize(bool newStaticSize);

    void setScale(qreal scale);
    void setPos(const QPointF &pos);
    void setZValue(qreal z);

    void setText(const QString &text);

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);

    Align getAlign() const;
    void setAlign(Align newAlign);

signals:
    void clicked();

private:
    QString title;
    bool chosen;
    bool staticSize;

    Align align;

    QGraphicsTextItem * text;
};

#endif // MenuItem_H
