#ifndef MENU_H
#define MENU_H

#include <QList>
#include <QPointF>
#include <QGraphicsItem>
#include <QSharedPointer>

#include "MenuItem.h"

class Menu
{
public:
    Menu();
    ~Menu();

    virtual void show(QSharedPointer<QGraphicsScene> scene) = 0;
    virtual void hide(QSharedPointer<QGraphicsScene> scene) = 0;

    const QString transition();

    const QString &getBackgroundImage() const;
    void setBackgroundImage(const QString &newBackgroundImage);

    const QString &getBoardImage() const;
    void setBoardImage(const QString &newBoardImage);

    void addNewItem(QSharedPointer<MenuItem> newItem);

    QSharedPointer<QGraphicsPixmapItem> getBoard() const;

    const QList<QSharedPointer<MenuItem>> &getListOfItems() const;

private:
    QList<QSharedPointer<MenuItem>> listOfItems;
    QString backgroundImage;
    QSharedPointer<QGraphicsPixmapItem> board;
    QString boardImage;
};

#endif // MENU_H
