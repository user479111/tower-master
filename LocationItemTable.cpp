#include "LocationItemTable.h"

#include <QDir>
#include <QPen>
#include <QDebug>

LocationItemTable::LocationItemTable(QGraphicsView * parent)
{
}

LocationItemTable::~LocationItemTable()
{
}

void LocationItemTable::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "press event";
}

void LocationItemTable::mouseReleaseEvent(QMouseEvent *event)
{

}

const QList<LocationItem *> &LocationItemTable::getLocations() const
{
    return locations;
}
