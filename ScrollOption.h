#ifndef SCROLLOPTION_H
#define SCROLLOPTION_H

#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QStringList>
#include <QObject>
#include <QPointF>

#include "MenuItem.h"

class ScrollOption : public QObject
{
    Q_OBJECT
public:
    explicit ScrollOption(QObject *parent = nullptr);
    ~ScrollOption();

    void setPos(const QPointF &pos);

    void setDescription(const QString &newDescription);

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);

    void addOption(const QString &option);

    MenuItem *getLeftArrow() const;
    MenuItem *getRightArrow() const;

    QGraphicsTextItem *getDescription() const;

    const QString &getTitle() const;
    void setTitle(const QString &newTitle);

    QString getOptionChoice() const;

    void setCurrentOption(const QString &optionName);

    int getCurrentOptionNumber() const;

private slots:
    void scrollLeft();
    void scrollRight();

private:
    QString title;
    QGraphicsTextItem * description;

    MenuItem * leftArrow;
    MenuItem * rightArrow;

    QGraphicsTextItem * currentOption;
    QStringList options;
    int currentOptionNumber;
    int longestOptionLength;
};

#endif // SCROLLOPTION_H
