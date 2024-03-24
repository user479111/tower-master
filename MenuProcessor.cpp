#include "MenuProcessor.h"

#include <QDebug>

MenuProcessor::MenuProcessor(Preferences * preferences,
                             QGraphicsScene * scene) :
    preferences(preferences),
    scene(scene),
    baseMenu(new BaseMenu(preferences)),
    battleMenu(new BattleMenu(preferences)),
    settingsMenu(new SettingsMenu(preferences)),
    choice(StayInMenu),
    locationChoice("")
{
    // do the preparation with all the XML parameters read
    battleMenu->prepare();

    // start with Base Menu
    currentMenu = baseMenu;

    // display menu
    currentMenu->show(scene);

    connect(baseMenu, &Menu::itemClicked, this, &MenuProcessor::processItemClick);
    connect(battleMenu, &Menu::itemClicked, this, &MenuProcessor::processItemClick);
    connect(settingsMenu, &Menu::itemClicked, this, &MenuProcessor::processItemClick);
}

MenuProcessor::~MenuProcessor()
{
    delete baseMenu;
    delete battleMenu;
    delete settingsMenu;
}

void MenuProcessor::processItemClick()
{
    if (currentMenu->transition().isEmpty()) {
        return;
    }

    currentMenu->hide(scene);

    if (dynamic_cast<BaseMenu*>(currentMenu)) {

        if (currentMenu->transition() == "company") {
            // Process CompanyMenu class.
        } else if (currentMenu->transition() == "battle") {
            currentMenu = battleMenu;
        } else if (currentMenu->transition() == "settings") {
            currentMenu = settingsMenu;
        } else if (currentMenu->transition() == "credits") {
            // Process CreditsMenu class.
        } else if (currentMenu->transition() == "quit") {

            choice = Quit;
            emit keyChoiseMade();

        } else {
            qDebug() << "Incorrect transition: " << currentMenu->transition();
        }

    } else if (dynamic_cast<BattleMenu*>(currentMenu)) {

        if (currentMenu->transition() == "back") {
            currentMenu = baseMenu;
        } else if (currentMenu->transition() == "load") {

            if (!(locationChoice = battleMenu->getLocationChoice()).isEmpty()) {
                choice = StartGame;
                emit  keyChoiseMade();
            }
        }

    } else if (dynamic_cast<SettingsMenu*>(currentMenu)) {

        if (currentMenu->transition() == "back") {
            currentMenu = baseMenu;
        } else if (currentMenu->transition() == "apply") {

            dynamic_cast<SettingsMenu*>(currentMenu)->applySettings();

            //companyMenu->resetItemsLanguage();
            baseMenu->resetItemsLanguage();
            battleMenu->resetItemsLanguage();
            settingsMenu->resetItemsLanguage();
            //creditsMenu->resetItemsLanguage();

            choice = ApplySettings;
            emit  keyChoiseMade();

            choice = StayInMenu;
        }

    } else {
        qDebug() << "Incorrect type";
    }

    if (choice == StayInMenu) {
        currentMenu->show(scene);
    }
}

const QString &MenuProcessor::getLocationChoice() const
{
    return locationChoice;
}

MenuProcessor::Choice MenuProcessor::getChoice() const
{
    return choice;
}
