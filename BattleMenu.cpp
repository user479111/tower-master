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


            QSharedPointer<LocationItem> location = QSharedPointer<LocationItem>(new LocationItem(subdir,
                                                       locationListWidth,
                                                       locationListFontSize,
                                                       QPointF(locationListPos.x(),
                                                               locationListPos.y() +
                                                                (locationListFontSize + locationListInterval) *
                                                                    locations.size()))); // Each new item should be lower that previous

            connect(location.get(), &LocationItem::clicked, this, &BattleMenu::processLocationsClick);

            locations.append(location);
        } else {
        }
    }
}

void BattleMenu::show(QSharedPointer<QGraphicsScene> scene)
{
    scene->setBackgroundBrush(QBrush(QImage(":/Data/Data/Menu/" + getBackgroundImage())));
    scene->addItem(getBoard().get());

    scene->addItem(mapPreview.get());
    scene->addItem(locationInfo.get());

    // show locations
    foreach (auto item, locations) {
        scene->addItem(item->getBackgroundRect().get());
        scene->addItem(item.get());
    }

    foreach (auto item, getListOfItems()) {
        item->setChosen(false);
        scene->addItem(item.get());
    }
}

void BattleMenu::hide(QSharedPointer<QGraphicsScene> scene)
{
    scene->removeItem(getBoard().get());

    scene->removeItem(mapPreview.get());
    scene->removeItem(locationInfo.get());

    // hide locations
    for (auto item : locations) {
        scene->removeItem(item->getBackgroundRect().get());
        scene->removeItem(item.get());
    }

    // hide location info

    // hide items
    for (auto item : getListOfItems()) {
        scene->removeItem(item.get());
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

        mapPreview->setPixmap(QPixmap(":/Data/Data/Locations/" + item->getDirectoryName() + "/" + item->getLocationImage()));

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
