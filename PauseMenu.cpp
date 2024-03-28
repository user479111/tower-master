#include "PauseMenu.h"

const QString PauseMenu::XML_FILE_NAME = "PauseMenu.xml";

PauseMenu::PauseMenu(const Preferences * preferences)
    : Menu(preferences, XML_FILE_NAME)
{
    // Prepare items
    for (auto item : getListOfItems()) {
        if (item->getTitle() == "resume") {
            connect(item, &MenuItem::clicked, this, &PauseMenu::processResumeClick);
        } else if (item->getTitle() == "restart") {
            connect(item, &MenuItem::clicked, this, &PauseMenu::processRestartClick);
        } else if (item->getTitle() == "main-menu") {
            connect(item, &MenuItem::clicked, this, &PauseMenu::processMainMenuClick);
        }
    }

}

PauseMenu::~PauseMenu()
{
}

void PauseMenu::show(QGraphicsScene *scene)
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
}

void PauseMenu::hide(QGraphicsScene *scene)
{
    scene->removeItem(this);

    scene->removeItem(getBoard());

    // remove items
    for (auto item : getListOfItems()) {
        item->hide(scene);
    }
}

void PauseMenu::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Hold the mousePressEvent to block the other interface items
    return;
}

void PauseMenu::processResumeClick()
{
    emit resumeClicked();
}

void PauseMenu::processMainMenuClick()
{
    emit mainMenuClicked();
}

void PauseMenu::processRestartClick()
{
    emit restartClicked();
}
