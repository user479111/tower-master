#include "ScrollOption.h"

#include <QDebug>

ScrollOption::ScrollOption(QObject *parent)
    : QObject{parent},
      title(""),
      description(new QGraphicsTextItem),
      leftArrow(new MenuItem),
      rightArrow(new MenuItem),
      currentOption(new QGraphicsTextItem),
      currentOptionNumber(0),
      longestOptionLength(0)
{
    connect(leftArrow, &MenuItem::clicked, this, &ScrollOption::scrollLeft);
    connect(rightArrow, &MenuItem::clicked, this, &ScrollOption::scrollRight);
}

ScrollOption::~ScrollOption()
{
    delete description;
    delete leftArrow;
    delete rightArrow;
    delete currentOption;
}

void ScrollOption::setPos(const QPointF &pos)
{
    description->setPos(pos);
    leftArrow->setPos(description->pos() + description->boundingRect().topRight());
    currentOption->setPos(leftArrow->pos() + leftArrow->boundingRect().topRight() * leftArrow->scale());
    rightArrow->setPos(currentOption->pos() + currentOption->boundingRect().topRight());
}

void ScrollOption::setDescription(const QString &newDescription)
{
    description->setDefaultTextColor(Qt::black);
    description->setFont(QFont("Helvetica [Cronyx]", 14 /* TODO: Take from Preferences */ , QFont::Bold));
    description->setPlainText(newDescription);

    setPos(description->pos());
}

void ScrollOption::show(QGraphicsScene *scene)
{
    scene->addItem(description);
    scene->addItem(leftArrow);
    scene->addItem(rightArrow);
    scene->addItem(currentOption);
}

void ScrollOption::hide(QGraphicsScene *scene)
{
    scene->removeItem(description);
    scene->removeItem(leftArrow);
    scene->removeItem(rightArrow);
    scene->removeItem(currentOption);
}

void ScrollOption::addOption(const QString &option)
{
    options.append(option);

    // Prepare distance between arrows for the longest option
    if (longestOptionLength < option.size()) {
        // Check if this is the first option
        auto currentOptionText = longestOptionLength > 0 ? currentOption->toPlainText() : "";

        longestOptionLength = option.size();
        currentOption->setPlainText(option);
        setPos(description->pos());

        if (!currentOptionText.isEmpty()) {
            currentOption->setPlainText(currentOptionText);
        }

        // Place the option on the center between the arrows
        currentOption->setPos(leftArrow->pos().x() + leftArrow->boundingRect().width() * leftArrow->scale() +
                              (rightArrow->pos().x() - leftArrow->pos().x() - leftArrow->boundingRect().width() *
                               leftArrow->scale()) / 2 -
                              currentOption->boundingRect().width() / 2, currentOption->pos().y());
    }
}

void ScrollOption::scrollLeft()
{
    currentOptionNumber = currentOptionNumber > 0 ? currentOptionNumber - 1 : options.size() - 1;
    currentOption->setPlainText(options.at(currentOptionNumber));

    // Place the option on the center between the arrows
    currentOption->setPos(leftArrow->pos().x() + leftArrow->boundingRect().width() * leftArrow->scale() +
                          (rightArrow->pos().x() - leftArrow->pos().x() - leftArrow->boundingRect().width() *
                           leftArrow->scale()) / 2 -
                          currentOption->boundingRect().width() / 2, currentOption->pos().y());
}

void ScrollOption::scrollRight()
{
    currentOptionNumber = currentOptionNumber + 1 < options.size() ? currentOptionNumber + 1 : 0;
    currentOption->setPlainText(options.at(currentOptionNumber));

    // Place the option on the center between the arrows
    currentOption->setPos(leftArrow->pos().x() + leftArrow->boundingRect().width() * leftArrow->scale() +
                          (rightArrow->pos().x() - leftArrow->pos().x() - leftArrow->boundingRect().width() *
                           leftArrow->scale()) / 2 -
                          currentOption->boundingRect().width() / 2, currentOption->pos().y());
}

int ScrollOption::getCurrentOptionNumber() const
{
    return currentOptionNumber;
}

const QString &ScrollOption::getTitle() const
{
    return title;
}

void ScrollOption::setTitle(const QString &newTitle)
{
    title = newTitle;
}

QString ScrollOption::getOptionChoise() const
{
    return currentOption->toPlainText();
}

void ScrollOption::setCurrentOption(const QString &optionName)
{
    currentOptionNumber = options.indexOf(optionName);

    if (currentOptionNumber < 0) {
        currentOptionNumber  = 0;
        return;
    }

    currentOption->setPlainText(options.at(currentOptionNumber));

    // Place the option on the center between the arrows
    currentOption->setPos(leftArrow->pos().x() + leftArrow->boundingRect().width() * leftArrow->scale() +
                          (rightArrow->pos().x() - leftArrow->pos().x() - leftArrow->boundingRect().width() *
                           leftArrow->scale()) / 2 -
                          currentOption->boundingRect().width() / 2, currentOption->pos().y());
}

QGraphicsTextItem *ScrollOption::getDescription() const
{
    return description;
}

MenuItem *ScrollOption::getRightArrow() const
{
    return rightArrow;
}

MenuItem *ScrollOption::getLeftArrow() const
{
    return leftArrow;
}
