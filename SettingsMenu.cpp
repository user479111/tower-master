#include "SettingsMenu.h"

SettingsMenu::SettingsMenu(QObject *parent)
    : QObject{parent}
{

}

SettingsMenu::~SettingsMenu()
{

}

void SettingsMenu::show(QGraphicsScene *scene)
{
    scene->setBackgroundBrush(QBrush(QImage(":/Data/Data/Menu/" + getBackgroundImage())));
    scene->addItem(getBoard());

    // show items
    for (auto item : getListOfItems()) {
        item->setChosen(false);
        item->show(scene);
    }
}

void SettingsMenu::hide(QGraphicsScene *scene)
{
    scene->removeItem(getBoard());

    // remove items
    for (auto item : getListOfItems()) {
        item->hide(scene);
    }
}
