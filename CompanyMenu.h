#ifndef COMPANYMENU_H
#define COMPANYMENU_H

#include "Menu.h"
#include "LevelInfo.h"
#include "Preferences.h"
#include "MenuTextItem.h"

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

    void setInfoPos(QPointF newInfoPos);

    void setInfoFontSize(float newInfoFontSize);

    void setLevelListPos(QPointF newLevelListPos);

    void setLevelListWidth(int newLevelListWidth);

    const int getLevelChoiceId() const;

protected:
    void loadXmlParameters();

public slots:
    void processLevelClick(MenuTextItem * Choice);

private:
    static const QString XML_FILE_NAME;
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

    int levelChoiceId;

};

#endif // COMPANYMENU_H
