#ifndef VICTORYMENU_H
#define VICTORYMENU_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QObject>

#include "Menu.h"
#include "Preferences.h"

class VictoryMenu: public Menu, public QGraphicsRectItem
{
    Q_OBJECT
public:
    VictoryMenu(const Preferences * preferences);
    ~VictoryMenu();

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);

public slots:
    void processMainMenuClick();
    void processRestartClick();

signals:
    void mainMenuClicked();
    void restartClicked();

private:
    static const QString XML_FILE_NAME;
    static const QString VICTORY_TEXT_EN;
    static const QString VICTORY_TEXT_UA;
    static const QString VICTORY_TEXT_RU;

    static const QString FONT_STYLE;
    static const int     FONT_SIZE;

    QGraphicsTextItem * victoryTextItem;
};
#endif // VICTORYMENU_H
