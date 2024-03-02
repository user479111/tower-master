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

void Menu::addNewItem(QSharedPointer<MenuItem> newItem)
{
    listOfItems.append(newItem);
}

QSharedPointer<QGraphicsPixmapItem> Menu::getBoard() const
{
    return board;
}

const QList<QSharedPointer<MenuItem>> &Menu::getListOfItems() const
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
