#ifndef BASEMENU_H
#define BASEMENU_H

#include "Menu.h"

#include <QGraphicsScene>
#include <QSharedPointer>

class BaseMenu : public Menu
{
public:
    BaseMenu();
    virtual ~BaseMenu();

    void show(QSharedPointer<QGraphicsScene> scene);
    void hide(QSharedPointer<QGraphicsScene> scene);
};

#endif // BASEMENU_H
