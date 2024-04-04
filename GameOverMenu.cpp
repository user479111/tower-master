#include "GameOverMenu.h"

const QString GameOverMenu::XML_FILE_NAME  = "GameOverMenu.xml";
const QString GameOverMenu::GAME_OVER_TEXT_EN = "Game Over!";
const QString GameOverMenu::GAME_OVER_TEXT_UA = "Поразка!";
const QString GameOverMenu::GAME_OVER_TEXT_RU = "Поражение!";

const QString GameOverMenu::FONT_STYLE = "Helvetica [Cronyx]";
const int     GameOverMenu::FONT_SIZE  = 15;

GameOverMenu::GameOverMenu(const Preferences * preferences)
    : Menu(preferences, XML_FILE_NAME),
      gameOverTextItem(new QGraphicsTextItem)
{
    if (preferences->getLanguage() == "English") {
        gameOverTextItem->setPlainText(GAME_OVER_TEXT_EN);
    } else if (preferences->getLanguage() == "Українська") {
        gameOverTextItem->setPlainText(GAME_OVER_TEXT_UA);
    } else if (preferences->getLanguage() == "Русский") {
        gameOverTextItem->setPlainText(GAME_OVER_TEXT_RU);
    }

    // Prepare items
    for (auto item : getListOfItems()) {
        if (item->getTitle() == "restart") {
            connect(item, &MenuItem::clicked, this, &GameOverMenu::processRestartClick);
        } else if (item->getTitle() == "main-menu") {
            connect(item, &MenuItem::clicked, this, &GameOverMenu::processMainMenuClick);
        }
    }
}

GameOverMenu::~GameOverMenu()
{
    delete gameOverTextItem;
}

void GameOverMenu::show(QGraphicsScene *scene)
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

    // Show the "Game Over" message
    gameOverTextItem->setDefaultTextColor(Qt::white);
    gameOverTextItem->setFont(QFont(FONT_STYLE, FONT_SIZE, QFont::Bold));
    gameOverTextItem->setPos(getBoard()->x() + getBoard()->boundingRect().width() / 2 - gameOverTextItem->boundingRect().width() / 2,
                             getListOfItems().first()->pos().y() - getListOfItems().first()->boundingRect().height());
    gameOverTextItem->setZValue(1);

    scene->addItem(gameOverTextItem);
}

void GameOverMenu::hide(QGraphicsScene *scene)
{
    scene->removeItem(this);

    scene->removeItem(getBoard());

    // remove items
    for (auto item : getListOfItems()) {
        item->hide(scene);
    }
}

void GameOverMenu::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Hold the mousePressEvent to block the other interface items
    return;
}

void GameOverMenu::processMainMenuClick()
{
    emit mainMenuClicked();
}

void GameOverMenu::processRestartClick()
{
    emit restartClicked();
}
