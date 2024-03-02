#ifndef GAMEPROCESSOR_H
#define GAMEPROCESSOR_H

#include <QObject>
#include <QGraphicsScene>
#include <QSharedPointer>
#include <QMouseEvent>

#include "Cursor.h"
#include "Location.h"
#include "GameInterface.h"
#include "Battlefield.h"

class GameProcessor : public QObject
{
    Q_OBJECT
public:
    explicit GameProcessor(QSharedPointer<QGraphicsScene> scene,
                           QString locationName,
                           QSharedPointer<Cursor> cursor,
                           QSharedPointer<QObject> parent = nullptr);

    ~GameProcessor();

private slots:
    void processMainMenuClick();

signals:
    void mainMenuSignal();

private:
    QSharedPointer<QGraphicsScene> scene;
    QSharedPointer<Cursor> cursor;

    QSharedPointer<Location> location;
    QSharedPointer<Battlefield> battlefield;

    QSharedPointer<GameInterface> gameInterface;
};

#endif // GAMEPROCESSOR_H
