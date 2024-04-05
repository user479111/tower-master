#ifndef COMPANYMENU_H
#define COMPANYMENU_H

#include "Menu.h"
#include "Preferences.h"
#include "LocationItem.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QObject>
#include <QList>

class CompanyMenu : public Menu
{
    Q_OBJECT
public:
    CompanyMenu(const Preferences * preferences);
    virtual ~CompanyMenu();

    void prepare();

    void show(QGraphicsScene * scene);
    void hide(QGraphicsScene * scene);

    void setLevelListFontSize(int newLevelListFontSize);

    void setLevelListInterval(int newLevelListInterval);

    void setMapPreviewImage(const QString &newMapPreviewImage);

    void setMapPreviewPos(QPointF newMapPreviewPos);

    void setMapPreviewScale(float newMapPreviewScale);

    void setLevelInfoPos(QPointF newLevelInfoPos);

    void setLevelInfoFontSize(float newLevelInfoFontSize);

    void setLevelListPos(QPointF newLevelListPos);

    void setLevelListWidth(int newLevelListWidth);

    const QString &getLevelChoice() const;

protected:
    void loadXmlParameters();

public slots:
    void processLevelClick();

private:
    static const QString XML_FILE_NAME;
    static const QString LOCATIONS_DIRECTORY;
    const Preferences * preferences;

    QList<LocationItem*> levels;
    QPointF levelListPos;
    int levelListFontSize;
    int levelListInterval;
    int levelListWidth;

    QGraphicsPixmapItem* mapPreview;
    QString mapPreviewImage;
    QPointF mapPreviewPos;
    float mapPreviewScale;

    QGraphicsTextItem* levelInfo;
    QPointF levelInfoPos;
    float levelInfoFontSize;

    QString levelChoice;
};

#endif // COMPANYMENU_H
