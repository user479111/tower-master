#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include <QGraphicsScene>

#include "Menu.h"
#include "Preferences.h"


class SettingsMenu : public Menu
{
public:
    SettingsMenu(Preferences * preferences);
    virtual ~SettingsMenu();

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);

    void matchSettings();
    void applySettings();

private:
    static const QString XML_FILE_NAME;
    Preferences * preferences;
};

#endif // SETTINGSMENU_H
