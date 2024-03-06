#ifndef BASEMENU_H
#define BASEMENU_H

#include "Menu.h"

#include <QGraphicsScene>

class BaseMenu : public Menu
{
public:
    BaseMenu();
    virtual ~BaseMenu();

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);
};

#endif // BASEMENU_H
