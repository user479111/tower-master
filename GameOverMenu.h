#ifndef GAMEOVERMENU_H
#define GAMEOVERMENU_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QObject>

#include "Menu.h"
#include "Preferences.h"

class GameOverMenu: public Menu, public QGraphicsRectItem
{
    Q_OBJECT
public:
    GameOverMenu(const Preferences * preferences);
    ~GameOverMenu();

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
    static const QString GAME_OVER_TEXT_EN;
    static const QString GAME_OVER_TEXT_UA;
    static const QString GAME_OVER_TEXT_RU;

    static const QString FONT_STYLE;
    static const int     FONT_SIZE;

    QGraphicsTextItem * gameOverTextItem;
};

#endif // GAMEOVERMENU_H
