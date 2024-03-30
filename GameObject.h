#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "QGraphicsPixmapItem"
#include <QObject>


class GameObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    GameObject(QObject *parent = nullptr);

signals:

};

#endif // GAMEOBJECT_H
