#ifndef MenuItem_H
#define MenuItem_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QObject>

class MenuItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    MenuItem(QGraphicsItem * parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    const QString &getTitle() const;
    void setTitle(const QString &newTitle);

    bool isChosen() const;
    void setChosen(bool newChosen);

    bool getStaticSize() const;
    void setStaticSize(bool newStaticSize);

signals:
    void clicked();

private:
    QString title;
    bool chosen;
    bool staticSize;
};

#endif // MenuItem_H
