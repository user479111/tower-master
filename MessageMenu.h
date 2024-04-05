#ifndef MESSAGEMENU_H
#define MESSAGEMENU_H


#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QObject>

#include "Menu.h"
#include "Preferences.h"

class MessageMenu : public Menu, public QGraphicsRectItem
{
    Q_OBJECT
public:
    MessageMenu(const Preferences * preferences);
    ~MessageMenu();

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    void setMessageText(const QString & messageText);

public slots:
    void processCloseClick();

signals:
    void closeClicked();

private:
    static const QString XML_FILE_NAME;

    static const QString FONT_STYLE;
    static const int     FONT_SIZE;
    static const float   SPACE_FACTOR;

    QGraphicsTextItem * messageTextItem;
};

#endif // MESSAGEMENU_H
