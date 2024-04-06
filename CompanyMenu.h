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
    void prepareLevel(const QString & id, bool active);

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

    void setLevelInfoWidth(int newLevelInfoWidth);

protected:
    void loadXmlParameters();

public slots:
    void processLevelClick(LocationItem * choise);

private:
    static const QString XML_FILE_NAME;
    static const QString SAVE_COMPANY_FILE;
    static const QString LOCATIONS_DIRECTORY;
    static const QString LEVEL_DIR_PREFIX;
    static const QString LEVEL_FILE_NAME;
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
    int levelInfoWidth;

    QString levelChoice;
};

#endif // COMPANYMENU_H
