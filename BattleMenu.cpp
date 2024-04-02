#include "BattleMenu.h"

#include <QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QColor>

const QString BattleMenu::XML_FILE_NAME = "BattleMenu.xml";
const QString BattleMenu::LOCATIONS_DIRECTORY = ":/Data/Data/Locations/";

BattleMenu::BattleMenu(const Preferences * preferences)
    : Menu(preferences, XML_FILE_NAME),
      preferences(preferences),
      mapPreview(new QGraphicsPixmapItem()),
      locationInfo(new QGraphicsTextItem()),
      locationChoice("")
{
    loadXmlParameters();
}

BattleMenu::~BattleMenu()
{
    for (auto locationItem : locations) {
        delete locationItem;
    }

    delete locationInfo;

    delete mapPreview;
}

void BattleMenu::prepare()
{
    mapPreview->setPixmap(QPixmap(MENU_DIRECTORY + mapPreviewImage));
    mapPreview->setPos(mapPreviewPos);
    mapPreview->setScale(mapPreviewScale);

    locationInfo->setPos(mapPreview->pos().x(),
                         mapPreview->pos().y() + mapPreview->boundingRect().width() * mapPreview->scale());
    locationInfo->setDefaultTextColor(Qt::black);
    locationInfo->setFont(QFont("Helvetica [Cronyx]", locationInfoFontSize, QFont::Medium));

    QStringList subdirectories = QDir(LOCATIONS_DIRECTORY).entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    foreach(const QString &subdir, subdirectories) {

        QFileInfo fileInfo(LOCATIONS_DIRECTORY + subdir + "/Location.xml");

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
    // Load image
    QPixmap backgroundImage(MENU_DIRECTORY + getBackgroundImage());
     // Scale it to the screen size
    backgroundImage = backgroundImage.scaled(scene->width(),
                                             scene->height(),
                                             Qt::IgnoreAspectRatio,
                                             Qt::SmoothTransformation);
    // Set it as a background brush
    scene->setBackgroundBrush(backgroundImage);

    // Display board with menu items
    scene->addItem(getBoard());

    // Display preview image of location map
    scene->addItem(mapPreview);

    // Display the information about location
    scene->addItem(locationInfo);

    // Show locations list
    foreach (auto item, locations) {
        scene->addItem(item->getBackgroundRect());
        scene->addItem(item);
    }

    // Show menu items
    foreach (auto item, getListOfItems()) {
        item->setChosen(false);
        item->show(scene);
    }
}

void BattleMenu::hide(QGraphicsScene * scene)
{
    scene->removeItem(getBoard());

    scene->removeItem(mapPreview);
    scene->removeItem(locationInfo);

    // Hide locations
    for (auto item : locations) {
        scene->removeItem(item->getBackgroundRect());
        scene->removeItem(item);
    }

    // Clear location info
    locationInfo->setPlainText("");

    // Hide items
    for (auto item : getListOfItems()) {
        item->hide(scene);
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

        mapPreview->setPixmap(QPixmap(LOCATIONS_DIRECTORY +
                                      item->getDirectoryName() +
                                      "/" +
                                      item->getLocationImage()));

        locationInfo->setPos(mapPreview->pos().x(),
                             mapPreview->pos().y() + mapPreview->boundingRect().height() * mapPreview->scale());

        if (preferences->getLanguage() == "English") {
            locationInfo->setPlainText("Name: " + item->getLocationFullName() + "\n"
                                       + "Vawes: " + QString::number(item->getWavesNum()));
        } else if (preferences->getLanguage() == "Українська") {
            locationInfo->setPlainText("Назва: " + item->getLocationFullName() + "\n"
                                       + "Хвилі: " + QString::number(item->getWavesNum()));
        } else if (preferences->getLanguage() == "Русский") {
            locationInfo->setPlainText("Название: " + item->getLocationFullName() + "\n"
                                       + "Волны: " + QString::number(item->getWavesNum()));
        }
    }
}

const QString &BattleMenu::getLocationChoice() const
{
    return locationChoice;
}

void BattleMenu::loadXmlParameters()
{
    QDomDocument MenuProcessorXml;

    QFile xmlFile(MENU_DIRECTORY + XML_FILE_NAME);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error while loading file: " << MENU_DIRECTORY + XML_FILE_NAME;
    }
    MenuProcessorXml.setContent(&xmlFile);
    xmlFile.close();

    auto root = MenuProcessorXml.documentElement();
    auto node = root.firstChild().toElement();

    while(!node.isNull())
    {
        if(QString(node.tagName()) != "section") {
            node = node.nextSibling().toElement();
            continue;
        }

        if (QString(node.attribute("name")) == "locations-list") {

            QDomNodeList menuSecondaryAttributes = node.childNodes();
            for (size_t j = 0, menuItemsNum = menuSecondaryAttributes.size(); j != menuItemsNum; j++) {
                auto currentElement = menuSecondaryAttributes.at(j).toElement();
                if(currentElement.tagName() == "attnum") {

                    if (QString(currentElement.attribute("name")).contains("font-size")) {

                        setLocationListFontSize(QString(currentElement.
                                                        attributes().
                                                        namedItem("val").
                                                        nodeValue()).
                                                toFloat());

                    } else if (QString(currentElement.attribute("name")).contains("interval")) {

                        setLocationListInterval(QString(currentElement.
                                                        attributes().
                                                        namedItem("val").
                                                        nodeValue()).
                                                toInt());

                    } else if (QString(currentElement.attribute("name")).contains("width")) {

                        setLocationListWidth(QString(currentElement.
                                                     attributes().
                                                     namedItem("val").
                                                     nodeValue()).
                                             toInt());


                    } else if (QString(currentElement.attribute("name")).contains("height")) {

                        // TODO : add scroll

                    }

                } else if (currentElement.tagName() == "attpos") {

                    // Set locations list coordinates
                    setLocationListPos(QPointF(QString(
                                                   currentElement.
                                                   attributes().
                                                   namedItem("x").
                                                   nodeValue()).
                                               toFloat(),
                                               QString(
                                                   currentElement.
                                                   attributes().
                                                   namedItem("y").
                                                   nodeValue()).
                                               toFloat()));

                }
            }

        } else if (QString(node.attribute("name")) == "map-preview") {

            QDomNodeList menuSecondaryAttributes = node.childNodes();
            for (size_t j = 0, menuItemsNum = menuSecondaryAttributes.size(); j != menuItemsNum; j++) {
                auto currentElement = menuSecondaryAttributes.at(j).toElement();

                if (currentElement.tagName() == "attstr") {

                    if (QString(currentElement.attribute("name")).contains("default-map")) {

                        // Set map preview source image
                        setMapPreviewImage(QString(currentElement.
                                                   attributes().
                                                   namedItem("val").
                                                   nodeValue()));

                    }

                } else if(currentElement.tagName() == "attnum") {

                    if (QString(currentElement.attribute("name")).contains("scale")) {

                        setMapPreviewScale(QString(currentElement.
                                                   attributes().
                                                   namedItem("val").
                                                   nodeValue()).
                                           toFloat());

                    }

                } else if (currentElement.tagName() == "attpos") {

                    // Set map preview coordinates
                    setMapPreviewPos(QPointF(QString(currentElement.
                                                     attributes().
                                                     namedItem("x").
                                                     nodeValue()).
                                             toFloat(),
                                             QString(
                                                 currentElement.
                                                 attributes().
                                                 namedItem("y").
                                                 nodeValue()).
                                             toFloat()));

                }
            }

        } else if (QString(node.attribute("name")) == "location-info") {

            QDomNodeList menuSecondaryAttributes = node.childNodes();
            for (size_t j = 0, menuItemsNum = menuSecondaryAttributes.size(); j != menuItemsNum; j++) {
                auto currentElement = menuSecondaryAttributes.at(j).toElement();

                if(currentElement.tagName() == "attnum") {

                    if (QString(currentElement.attribute("name")).contains("font-size")) {

                        setLocationInfoFontSize(QString(currentElement.
                                                        attributes().
                                                        namedItem("val").
                                                        nodeValue()).
                                                toFloat());

                    }

                } else if (currentElement.tagName() == "attpos") {

                    // Set location information coordinates
                    setLocationInfoPos(QPointF(QString(currentElement.
                                                       attributes().
                                                       namedItem("x").
                                                       nodeValue()).
                                               toFloat(),
                                               QString(currentElement.
                                                       attributes().
                                                       namedItem("y").
                                                       nodeValue()).
                                               toFloat()));

                }
            }

        }
        node = node.nextSibling().toElement();
    }
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
