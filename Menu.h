#ifndef MENU_H
#define MENU_H

#include <QList>
#include <QPointF>
#include <QGraphicsItem>

#include "MenuItem.h"

class Menu
{
public:
    Menu();
    ~Menu();

    virtual void show(QGraphicsScene * scene) = 0;
    virtual void hide(QGraphicsScene * scene) = 0;

    const QString transition();

    const QString &getBackgroundImage() const;
    void setBackgroundImage(const QString &newBackgroundImage);

    const QString &getBoardImage() const;
    void setBoardImage(const QString &newBoardImage);

    void addNewItem(MenuItem * newItem);

    QGraphicsPixmapItem * getBoard() const;

    const QList<MenuItem*> &getListOfItems() const;

private:
    QList<MenuItem*> listOfItems;
    QString backgroundImage;
    QGraphicsPixmapItem * board;
    QString boardImage;
};

#endif // MENU_H
