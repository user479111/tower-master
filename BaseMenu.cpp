#include "BaseMenu.h"

#include <QDebug>


const QString BaseMenu::XML_FILE_NAME = "BaseMenu.xml";

BaseMenu::BaseMenu(const Preferences * preferences)
    : Menu(preferences, XML_FILE_NAME)
{
}

BaseMenu::~BaseMenu()
{
}

void BaseMenu::show(QGraphicsScene * scene)
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
}

void BaseMenu::hide(QGraphicsScene * scene)
{
    scene->removeItem(getBoard());

    // remove items
    for (auto item : getListOfItems()) {
        item->hide(scene);
    }
}

