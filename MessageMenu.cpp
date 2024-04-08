#include "MessageMenu.h"

#include <QTextDocument>
#include <QTextOption>

const QString MessageMenu::XML_FILE_NAME  = "MessageMenu.xml";

const QString MessageMenu::FONT_STYLE = "Helvetica [Cronyx]";
const int     MessageMenu::FONT_SIZE  = 15;
const float   MessageMenu::SPACE_FACTOR = 0.8;

MessageMenu::MessageMenu(const Preferences * preferences)
    : Menu(preferences, XML_FILE_NAME),
      messageTextItem(new QGraphicsTextItem)
{
    // Prepare items
    for (auto item : getListOfItems()) {
        if (item->getTitle() == "close") {
            connect(item, &MenuItem::clicked, this, &MessageMenu::processCloseClick);
        }
    }

    messageTextItem->setTextWidth(getBoard()->boundingRect().width() * SPACE_FACTOR);
    messageTextItem->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
}

MessageMenu::~MessageMenu()
{
    delete messageTextItem;
}

void MessageMenu::show(QGraphicsScene *scene)
{
    // Cover everything on the back with gray covering
    setRect(scene->sceneRect());
    setPen(QPen(QColor(255, 255, 255, 255)));
    setBrush(QBrush(QColor(0, 0, 0, 100)));
    setZValue(1); // the background should be over everything else

    scene->addItem(this);

    // Place board on the top-center of the screen
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

    // Show the Message
    messageTextItem->setDefaultTextColor(Qt::black);
    messageTextItem->setFont(QFont(FONT_STYLE, FONT_SIZE, QFont::Bold));
    messageTextItem->setPos(getBoard()->x() + getBoard()->boundingRect().width() / 2 - messageTextItem->boundingRect().width() / 2,
                             getBoard()->y() + getBoard()->boundingRect().height() / 2 - messageTextItem->boundingRect().height() / 2);
    messageTextItem->setZValue(1);

    scene->addItem(messageTextItem);
}

void MessageMenu::hide(QGraphicsScene *scene)
{
    scene->removeItem(this);

    scene->removeItem(getBoard());

    scene->removeItem(messageTextItem);

    // remove items
    for (auto item : getListOfItems()) {
        item->hide(scene);
    }
}

void MessageMenu::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Hold the mousePressEvent to block the other interface items
    return;
}

void MessageMenu::setMessageText(const QString &messageText)
{
    messageTextItem->setPlainText(messageText);
}

void MessageMenu::processCloseClick()
{
    emit closeClicked();
}
