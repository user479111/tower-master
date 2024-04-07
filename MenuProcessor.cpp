#include "MenuProcessor.h"

#include <QDebug>

MenuProcessor::MenuProcessor(Preferences * preferences,
                             QGraphicsScene * scene) :
    preferences(preferences),
    scene(scene),
    baseMenu(new BaseMenu(preferences)),
    companyMenu(new CompanyMenu(preferences)),
    battleMenu(new BattleMenu(preferences)),
    settingsMenu(new SettingsMenu(preferences)),
    choice(StayInMenu),
    levelChoiceId(0)
{
    // start with Base Menu
    currentMenu = baseMenu;

    // display menu
    currentMenu->show(scene);

    connect(baseMenu, &Menu::itemClicked, this, &MenuProcessor::processItemClick);
    connect(companyMenu, &Menu::itemClicked, this, &MenuProcessor::processItemClick);
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
            currentMenu = companyMenu;
        } else if (currentMenu->transition() == "battle") {
            currentMenu = battleMenu;
        } else if (currentMenu->transition() == "settings") {
            currentMenu = settingsMenu;
        } else if (currentMenu->transition() == "credits") {
            // Process CreditsMenu class.
        } else if (currentMenu->transition() == "quit") {

            choice = Quit;
            emit keyChoiceMade();

        } else {
            qDebug() << "Incorrect transition: " << currentMenu->transition();
        }

    } else if (dynamic_cast<CompanyMenu*>(currentMenu)) {

        if (currentMenu->transition() == "back") {
            currentMenu = baseMenu;
        } else if (currentMenu->transition() == "load") {
            levelChoiceId = companyMenu->getLevelChoiceId();
            choice = StartGame;
            preferences->setGameMode(Preferences::GameMode::Company);
            emit  keyChoiceMade();
        }

    } else if (dynamic_cast<BattleMenu*>(currentMenu)) {

        if (currentMenu->transition() == "back") {
            currentMenu = baseMenu;
        } else if (currentMenu->transition() == "load") {
            levelChoiceId = battleMenu->getLevelChoiceId();
            choice = StartGame;
            preferences->setGameMode(Preferences::GameMode::Battle);
            emit  keyChoiceMade();
        }

    } else if (dynamic_cast<SettingsMenu*>(currentMenu)) {

        if (currentMenu->transition() == "back") {
            currentMenu = baseMenu;
        } else if (currentMenu->transition() == "apply") {

            dynamic_cast<SettingsMenu*>(currentMenu)->applySettings();

            companyMenu->resetItemsLanguage();
            baseMenu->resetItemsLanguage();
            battleMenu->resetItemsLanguage();
            settingsMenu->resetItemsLanguage();
            //creditsMenu->resetItemsLanguage();

            choice = ApplySettings;
            emit  keyChoiceMade();

            choice = StayInMenu;
        }

    } else {
        qDebug() << "Incorrect type";
    }

    if (choice == StayInMenu) {
        currentMenu->show(scene);
    }
}

int MenuProcessor::getLevelChoiceId() const
{
    return levelChoiceId;
}

MenuProcessor::Choice MenuProcessor::getChoice() const
{
    return choice;
}
