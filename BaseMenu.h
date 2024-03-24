#ifndef BASEMENU_H
#define BASEMENU_H

#include "Menu.h"
#include "Preferences.h"

#include <QGraphicsScene>
#include <QString>


class BaseMenu : public Menu
{
public:
    BaseMenu(const Preferences * preferences);
    virtual ~BaseMenu();

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);
private:
    static const QString XML_FILE_NAME;
};

#endif // BASEMENU_H
