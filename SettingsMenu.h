#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include <QGraphicsScene>
#include <QObject>

#include "Menu.h"

class SettingsMenu : public QObject, public Menu
{
    Q_OBJECT
public:
    explicit SettingsMenu(QObject *parent = nullptr);
    virtual ~SettingsMenu();

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);

signals:

};

#endif // SETTINGSMENU_H
