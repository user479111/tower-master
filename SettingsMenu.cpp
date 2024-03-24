#include "SettingsMenu.h"

#include <QDebug>

const QString SettingsMenu::XML_FILE_NAME = "SettingsMenu.xml";

SettingsMenu::SettingsMenu(Preferences * preferences)
    : Menu(preferences, XML_FILE_NAME),
      preferences(preferences)
{
}

SettingsMenu::~SettingsMenu()
{
}

void SettingsMenu::show(QGraphicsScene *scene)
{
    // Load image
    QPixmap backgroundImage(MENU_DIRECTORY + getBackgroundImage());
     // Scale it to the screen size
    backgroundImage = backgroundImage.scaled(scene->width(),
                                             scene->height(),
                                             Qt::IgnoreAspectRatio,
                                             Qt::SmoothTransformation);
    // Set it as a background brush
    scene->setBackgroundBrush(backgroundImage);

    // Display board with menu items
    scene->addItem(getBoard());

    // Show items
    for (auto item : getListOfItems()) {
        item->setChosen(false);
        item->show(scene);
    }

    // Set options from prferences
    matchSettings();

    // Show options
    for (auto option : getListOfOptions()) {
        option->show(scene);
    }
}

void SettingsMenu::hide(QGraphicsScene *scene)
{
    scene->removeItem(getBoard());

    // remove items
    for (auto item : getListOfItems()) {
        item->hide(scene);
    }

    // hide options
    for (auto item : getListOfOptions()) {
        item->hide(scene);
    }
}

// Set options from prferences
void SettingsMenu::matchSettings()
{
    for (auto option : getListOfOptions()) {

        if (option->getTitle() == "fullscreen-mode") {
            option->setCurrentOption(preferences->getFullscreenString());
        } else if (option->getTitle() == "resolution") {
            option->setCurrentOption(preferences->getResolutionString());
        } else if (option->getTitle() == "language") {
            option->setCurrentOption(preferences->getLanguage());
        }
    }
}

// The applySettings function only apply changes on preferences
// All of the updates on application is performed in the
// TowerMaster class
void SettingsMenu::applySettings()
{
    preferences->saveNewOptions(getListOfOptions());
}
