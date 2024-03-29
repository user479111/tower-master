#include "VictoryMenu.h"

const QString VictoryMenu::XML_FILE_NAME  = "VictoryMenu.xml";
const QString VictoryMenu::VICTORY_TEXT = "You win!";

const QString VictoryMenu::FONT_STYLE = "Helvetica [Cronyx]";
const int     VictoryMenu::FONT_SIZE  = 15;

VictoryMenu::VictoryMenu(const Preferences * preferences)
    : Menu(preferences, XML_FILE_NAME),
      victoryTextItem(new QGraphicsTextItem)
{
    // Prepare items
    for (auto item : getListOfItems()) {
        if (item->getTitle() == "restart") {
            connect(item, &MenuItem::clicked, this, &VictoryMenu::processRestartClick);
        } else if (item->getTitle() == "main-menu") {
            connect(item, &MenuItem::clicked, this, &VictoryMenu::processMainMenuClick);
        }
    }
}

VictoryMenu::~VictoryMenu()
{
    delete victoryTextItem;
}

void VictoryMenu::show(QGraphicsScene *scene)
{
    setRect(scene->sceneRect());
    setPen(QPen(QColor(255, 255, 255, 255)));
    setBrush(QBrush(QColor(0, 0, 0, 100)));
    setZValue(1); // the background should be over everything else

    scene->addItem(this);

    // Place board on the up-center of the screen
    setBoardPos(scene->sceneRect().x() +
                scene->sceneRect().width() / 2 -
                getBoard()->boundingRect().width() / 2,
                scene->sceneRect().y());

    // Display board with menu items
    scene->addItem(getBoard());

    // Show items
    for (auto item : getListOfItems()) {
        item->setChosen(false);
        item->show(scene);
    }

    // Show the "You win!" message
    victoryTextItem->setPlainText(VICTORY_TEXT);
    victoryTextItem->setDefaultTextColor(Qt::white);
    victoryTextItem->setFont(QFont(FONT_STYLE, FONT_SIZE, QFont::Bold));
    victoryTextItem->setPos(getBoard()->x() + getBoard()->boundingRect().width() / 2 - victoryTextItem->boundingRect().width() / 2,
                             getListOfItems().first()->pos().y() - getListOfItems().first()->boundingRect().height());
    victoryTextItem->setZValue(1);

    scene->addItem(victoryTextItem);
}

void VictoryMenu::hide(QGraphicsScene *scene)
{
    scene->removeItem(this);

    scene->removeItem(getBoard());

    // remove items
    for (auto item : getListOfItems()) {
        item->hide(scene);
    }
}

void VictoryMenu::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Hold the mousePressEvent to block the other interface items
    return;
}

void VictoryMenu::processMainMenuClick()
{
    emit mainMenuClicked();
}

void VictoryMenu::processRestartClick()
{
    emit restartClicked();
}
