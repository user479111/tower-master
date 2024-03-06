#include "BaseMenu.h"

#include <QDebug>

BaseMenu::BaseMenu()
{
}

BaseMenu::~BaseMenu()
{
}

void BaseMenu::show(QGraphicsScene * scene)
{
    qDebug() << getBackgroundImage();
    scene->setBackgroundBrush(QBrush(QImage(":/Data/Data/Menu/" + getBackgroundImage())));

    scene->addItem(getBoard());

    // show items
    for (auto item : getListOfItems()) {
        item->setChosen(false);
        scene->addItem(item);
    }
}

void BaseMenu::hide(QGraphicsScene * scene)
{
    scene->removeItem(getBoard());

    // remove items
    for (auto item : getListOfItems()) {
        scene->removeItem(item);
    }
}

