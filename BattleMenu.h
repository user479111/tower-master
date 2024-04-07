#ifndef BATTLEMENU_H
#define BATTLEMENU_H

#include "Menu.h"
#include "LevelInfo.h"
#include "Preferences.h"
#include "MenuTextItem.h"

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

    void setLevelListFontSize(int newLevelListFontSize);

    void setLevelListInterval(int newLevelListInterval);

    void setMapPreviewImage(const QString &newMapPreviewImage);

    void setMapPreviewPos(QPointF newMapPreviewPos);

    void setMapPreviewScale(float newMapPreviewScale);

    void setInfoPos(QPointF newInfoPos);

    void setInfoFontSize(float newInfoFontSize);

    void setLevelListPos(QPointF newLevelListPos);

    void setLevelListWidth(int newLevelListWidth);

    const int getLevelChoiceId() const;

    void setInfoWidth(int newInfoWidth);

protected:
    void loadXmlParameters();

public slots:
    void processLevelClick(MenuTextItem * Choice);

private:
    static const QString XML_FILE_NAME;
    static const QString LOCATIONS_DIRECTORY;
    static const QString SAVE_COMPANY_FILE;
    const Preferences * preferences;

    QMap<LevelInfo *, MenuTextItem *> levels;
    QPointF levelListPos;
    int levelListFontSize;
    int levelListInterval;
    int levelListWidth;

    QGraphicsPixmapItem* mapPreview;
    QString mapPreviewImage;
    QPointF mapPreviewPos;
    float mapPreviewScale;

    QGraphicsTextItem* info;
    QPointF infoPos;
    float infoFontSize;
    int infoWidth;

    int levelChoiceId;
};

#endif // BATTLEMENU_H
