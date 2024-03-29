#ifndef BATTLEMENU_H
#define BATTLEMENU_H

#include "Menu.h"
#include "Preferences.h"
#include "LocationItem.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QObject>
#include <QList>

class BattleMenu : public Menu
{
    Q_OBJECT
public:
    BattleMenu(const Preferences * preferences);
    virtual ~BattleMenu();

    void prepare();

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);

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

protected:
    void loadXmlParameters();

public slots:
    void processLocationsClick();

private:
    static const QString XML_FILE_NAME;
    static const QString LOCATIONS_DIRECTORY;
    const Preferences * preferences;

    QList<LocationItem*> locations;
    QPointF locationListPos;
    int locationListFontSize;
    int locationListInterval;
    int locationListWidth;

    QGraphicsPixmapItem* mapPreview;
    QString mapPreviewImage;
    QPointF mapPreviewPos;
    float mapPreviewScale;

    QGraphicsTextItem* locationInfo;
    QPointF locationInfoPos;
    float locationInfoFontSize;

    QString locationChoice;
};

#endif // BATTLEMENU_H
