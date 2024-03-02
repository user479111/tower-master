#ifndef LOCATIONITEMTABLE_H
#define LOCATIONITEMTABLE_H

#include "LocationItem.h"

#include <QGraphicsView>
#include <QList>

class LocationItemTable : public QGraphicsView
{
public:
    LocationItemTable(QGraphicsView *parent = 0);
    ~LocationItemTable();

    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

    const QList<LocationItem *> &getLocations() const;

private:
    QList<LocationItem*> locations;
};

#endif // LOCATIONITEMTABLE_H
