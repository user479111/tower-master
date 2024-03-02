#include "BaseMenu.h"

#include <QDebug>

BaseMenu::BaseMenu()
{
}

BaseMenu::~BaseMenu()
{
}

void BaseMenu::show(QSharedPointer<QGraphicsScene> scene)
{
    qDebug() << getBackgroundImage();
    scene->setBackgroundBrush(QBrush(QImage(":/Data/Data/Menu/" + getBackgroundImage())));

    scene->addItem(getBoard().get());

    // show items
    for (auto item : getListOfItems()) {
        item->setChosen(false);
        scene->addItem(item.get());
    }
}

void BaseMenu::hide(QSharedPointer<QGraphicsScene> scene)
{
    scene->removeItem(getBoard().get());

    // remove items
    for (auto item : getListOfItems()) {
        scene->removeItem(item.get());
    }
}

