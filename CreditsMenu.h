#ifndef CREDITSMENU_H
#define CREDITSMENU_H

#include <QList>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

#include "Menu.h"
#include "Preferences.h"

class CreditsMenu : public Menu
{
public:
    CreditsMenu(Preferences * preferences);
    virtual ~CreditsMenu();

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);

protected:
    void loadXmlParameters();

private:
    static const QString XML_FILE_NAME;

    static const QString FONT_STYLE;
    static const int     FONT_SIZE;
    static const float   SPACE_FACTOR;

    Preferences * preferences;

    QList<QGraphicsTextItem *> credits;
};

#endif // CREDITSMENU_H
