#include "Menu.h"

#include <QDebug>

Menu::Menu() :
    backgroundImage(""),
    board(new QGraphicsPixmapItem()),
    boardImage("")
{
}

Menu::~Menu()
{
    delete board;

    for (auto item : listOfItems) {
        delete item;
    }
    listOfItems.clear();
}

const QString &Menu::getBackgroundImage() const
{
    return backgroundImage;
}

void Menu::setBackgroundImage(const QString &newBackgroundImage)
{
    backgroundImage = newBackgroundImage;
}

const QString &Menu::getBoardImage() const
{
    return boardImage;
}

void Menu::setBoardImage(const QString &newBoardImage)
{
    boardImage = newBoardImage;

    board->setPixmap(":/Data/Data/Menu/" + boardImage);
}

void Menu::addNewItem(MenuItem * newItem)
{
    listOfItems.append(newItem);
}

QGraphicsPixmapItem* Menu::getBoard() const
{
    return board;
}

const QList<MenuItem*> &Menu::getListOfItems() const
{
    return listOfItems;
}

const QString Menu::transition()
{
    foreach (auto item, getListOfItems()) {

        if (!item->isChosen())
        {
            continue;
        }

        return item->getTitle();
    }

    return "";
}
