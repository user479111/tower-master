#include "BattleMenu.h"

#include <QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QColor>

BattleMenu::BattleMenu() :
    mapPreview(new QGraphicsPixmapItem()),
    locationInfo(new QGraphicsTextItem()),
    locationChoice("")
{
}

BattleMenu::~BattleMenu()
{
    delete mapPreview;
    delete locationInfo;

    for (auto locationItem : locations) {
        delete locationItem;
    }
}

void BattleMenu::prepare()
{
    mapPreview->setPixmap(QPixmap(":/Data/Data/Menu/" + mapPreviewImage));
    mapPreview->setPos(mapPreviewPos);
    mapPreview->setScale(mapPreviewScale);

    locationInfo->setPos(locationInfoPos);
    locationInfo->setDefaultTextColor(Qt::black);
    locationInfo->setFont(QFont("Helvetica [Cronyx]", locationInfoFontSize, QFont::Medium));

    QStringList subdirectories = QDir(":/Data/Data/Locations/").entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    foreach(const QString &subdir, subdirectories) {

        QFileInfo fileInfo(":/Data/Data/Locations/" + subdir + "/Location.xml");

        if (fileInfo.exists() && fileInfo.isFile()) {


            LocationItem * location = new LocationItem(subdir,
                                                       locationListWidth,
                                                       locationListFontSize,
                                                       QPointF(locationListPos.x(),
                                                               locationListPos.y() +
                                                                (locationListFontSize + locationListInterval) *
                                                                    locations.size())); // Each new item should be lower that previous

            connect(location, &LocationItem::clicked, this, &BattleMenu::processLocationsClick);

            locations.append(location);
        } else {
        }
    }
}

void BattleMenu::show(QGraphicsScene * scene)
{
    scene->setBackgroundBrush(QBrush(QImage(":/Data/Data/Menu/" + getBackgroundImage())));
    scene->addItem(getBoard());

    scene->addItem(mapPreview);
    scene->addItem(locationInfo);

    // show locations
    foreach (auto item, locations) {
        scene->addItem(item->getBackgroundRect());
        scene->addItem(item);
    }

    foreach (auto item, getListOfItems()) {
        item->setChosen(false);
        scene->addItem(item);
    }
}

void BattleMenu::hide(QGraphicsScene * scene)
{
    scene->removeItem(getBoard());

    scene->removeItem(mapPreview);
    scene->removeItem(locationInfo);

    // hide locations
    for (auto item : locations) {
        scene->removeItem(item->getBackgroundRect());
        scene->removeItem(item);
    }

    // hide location info

    // hide items
    for (auto item : getListOfItems()) {
        scene->removeItem(item);
    }
}

void BattleMenu::processLocationsClick()
{
    foreach (auto item, locations) {
        if (!item->isChosen())
        {
            continue;
        }

        foreach (auto subitem, locations) {
            subitem->setDefaultTextColor(Qt::black);
            subitem->getBackgroundRect()->setBrush(Qt::transparent);
        }

        item->setChosen(false);

        item->setDefaultTextColor(Qt::white);
        item->getBackgroundRect()->setBrush(QColor(153, 76, 0));

        locationChoice = item->getDirectoryName();

        mapPreview->setPixmap(QPixmap(":/Data/Data/Locations/" +
                                      item->getDirectoryName() +
                                      "/" +
                                      item->getLocationImage()));

        locationInfo->setPlainText("Name: " + item->getLocationFullName() + "\n"
                                   + "Vawes: " + QString::number(item->getWavesNum()));
    }
}

const QString &BattleMenu::getLocationChoice() const
{
    return locationChoice;
}

void BattleMenu::setLocationListWidth(int newLocationListWidth)
{
    locationListWidth = newLocationListWidth;
}

void BattleMenu::setLocationListPos(QPointF newLocationListPos)
{
    locationListPos = newLocationListPos;
}

void BattleMenu::setLocationListInterval(int newLocationListInterval)
{
    locationListInterval = newLocationListInterval;
}

void BattleMenu::setLocationInfoFontSize(float newLocationInfoFontSize)
{
    locationInfoFontSize = newLocationInfoFontSize;
}

void BattleMenu::setLocationInfoPos(QPointF newLocationInfoPos)
{
    locationInfoPos = newLocationInfoPos;
}

void BattleMenu::setMapPreviewScale(float newMapPreviewScale)
{
    mapPreviewScale = newMapPreviewScale;
}

void BattleMenu::setMapPreviewPos(QPointF newMapPreviewPos)
{
    mapPreviewPos = newMapPreviewPos;
}

void BattleMenu::setMapPreviewImage(const QString &newMapPreviewImage)
{
    mapPreviewImage = newMapPreviewImage;
}

void BattleMenu::setLocationListFontSize(int newLocationListFontSize)
{
    locationListFontSize = newLocationListFontSize;
}
