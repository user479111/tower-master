#ifndef BATTLEMENU_H
#define BATTLEMENU_H

#include "Menu.h"
#include "LocationItem.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QSharedPointer>
#include <QObject>
#include <QList>

class BattleMenu : public QObject, public Menu
{
    Q_OBJECT
public:
    BattleMenu();
    virtual ~BattleMenu();

    void prepare();

    void show(QSharedPointer<QGraphicsScene> scene);
    void hide(QSharedPointer<QGraphicsScene> scene);

    void setLocationListFontSize(int newLocationListFontSize);

    void setLocationListInterval(int newLocationListInterval);

    void setMapPreviewImage(const QString &newMapPreviewImage);

    void setMapPreviewPos(QPointF newMapPreviewPos);

    void setMapPreviewScale(float newMapPreviewScale);

    void setLocationInfoPos(QPointF newLocationInfoPos);

    void setLocationInfoFontSize(float newLocationInfoFontSize);

    void setLocationListPos(QPointF newLocationListPos);

    void setLocationListWidth(int newLocationListWidth);

    const QString &getLocationChoice() const;

public slots:
    void processLocationsClick();

private:
    QList<QSharedPointer<LocationItem>> locations;
    QPointF locationListPos;
    int locationListFontSize;
    int locationListInterval;
    int locationListWidth;

    QSharedPointer<QGraphicsPixmapItem> mapPreview;
    QString mapPreviewImage;
    QPointF mapPreviewPos;
    float mapPreviewScale;

    QSharedPointer<QGraphicsTextItem> locationInfo;
    QPointF locationInfoPos;
    float locationInfoFontSize;

    QString locationChoice;
};

#endif // BATTLEMENU_H
