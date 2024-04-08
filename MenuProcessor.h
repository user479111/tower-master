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
#include "CompanyMenu.h"
#include "BattleMenu.h"
#include "Preferences.h"
#include "SettingsMenu.h"
#include "CreditsMenu.h"

class MenuProcessor : public QObject
{
    Q_OBJECT
public:
    MenuProcessor(Preferences * preferences,
                  QGraphicsScene * scene);
    ~MenuProcessor();

    enum Choice { StayInMenu, StartGame, ApplySettings, Quit };
    Choice getChoice() const;

    int getLevelChoiceId() const;

private slots:
    void processItemClick();

signals:
    void keyChoiceMade();

private:
    Preferences * preferences;
    QGraphicsScene * scene;

    Menu * currentMenu;
    BaseMenu * baseMenu;
    CompanyMenu * companyMenu;
    BattleMenu * battleMenu;
    SettingsMenu * settingsMenu;
    CreditsMenu * creditsMenu;

    Choice choice;
    int levelChoiceId;
};

#endif // MENUPROCESSOR_H
