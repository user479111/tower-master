#ifndef MENUPROCESSOR_H
#define MENUPROCESSOR_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QObject>
#include <QPointF>
#include <QList>

#include "Menu.h"
#include "BaseMenu.h"
#include "BattleMenu.h"
#include "Preferences.h"
#include "SettingsMenu.h"

class MenuProcessor : public QObject
{
    Q_OBJECT
public:
    MenuProcessor(Preferences * preferences,
                  QGraphicsScene * scene);
    ~MenuProcessor();

    enum Choice { StayInMenu, StartGame, ApplySettings, Quit };
    Choice getChoice() const;

    const QString &getLocationChoice() const;

private slots:
    void processItemClick();

signals:
    void keyChoiseMade();

private:
    Preferences * preferences;
    QGraphicsScene * scene;

    Menu * currentMenu;
    BaseMenu * baseMenu;
    //CompanyMenu * subMenuCompany;
    BattleMenu * battleMenu;
    SettingsMenu * settingsMenu;
    //CreditsMenu * subMenuCreadits;

    Choice choice;
    QString locationChoice;
};

#endif // MENUPROCESSOR_H
