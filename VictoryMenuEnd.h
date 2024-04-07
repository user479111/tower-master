#ifndef VICTORYMENUEND_H
#define VICTORYMENUEND_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QObject>

#include "Menu.h"
#include "Preferences.h"

class VictoryMenuEnd: public Menu, public QGraphicsRectItem
{
    Q_OBJECT
public:
    VictoryMenuEnd(const Preferences * preferences);
    ~VictoryMenuEnd();

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    void prepare();

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
#endif // VICTORYMENUEND_H
