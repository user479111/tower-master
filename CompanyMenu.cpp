#include "CompanyMenu.h"

#include <QStringList>
#include <QFileInfo>
#include <QDebug>
#include <QColor>

const QString CompanyMenu::XML_FILE_NAME = "BattleMenu.xml";
const QString CompanyMenu::LOCATIONS_DIRECTORY = ":/Data/Data/Locations/";

CompanyMenu::CompanyMenu(const Preferences *preferences)
  : Menu(preferences, XML_FILE_NAME),
    preferences(preferences),
    mapPreview(new QGraphicsPixmapItem()),
    levelInfo(new QGraphicsTextItem()),
    levelChoice("")
{
    loadXmlParameters();

    // do the preparation with all the XML parameters read
    prepare();
}

CompanyMenu::~CompanyMenu()
{
    for (auto levelItem : levels) {
        delete levelItem;
    }

    delete levelInfo;

    delete mapPreview;
}

void CompanyMenu::prepare()
{

}

void CompanyMenu::show(QGraphicsScene *scene)
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
    scene->addItem(levelInfo);

    // Show locations list
    foreach (auto level, levels) {
        scene->addItem(level->getBackgroundRect());
        scene->addItem(level);
    }

    // Show menu items
    foreach (auto item, getListOfItems()) {
        item->setChosen(false);
        item->show(scene);
    }
}

void CompanyMenu::hide(QGraphicsScene *scene)
{
    scene->removeItem(getBoard());

    scene->removeItem(mapPreview);
    scene->removeItem(levelInfo);

    // Hide locations
    for (auto level : levels) {
        scene->removeItem(level->getBackgroundRect());
        scene->removeItem(level);
    }

    // Clear location info
    levelInfo->setPlainText("");

    // Hide items
    for (auto item : getListOfItems()) {
        item->hide(scene);
    }
}

void CompanyMenu::setLevelListFontSize(int newLevelListFontSize)
{
    levelListFontSize = newLevelListFontSize;
}

void CompanyMenu::setLevelListInterval(int newLevelListInterval)
{
    levelListInterval = newLevelListInterval;
}

void CompanyMenu::setMapPreviewImage(const QString &newMapPreviewImage)
{
    mapPreviewImage = newMapPreviewImage;
}

void CompanyMenu::setMapPreviewPos(QPointF newMapPreviewPos)
{
    mapPreviewPos = newMapPreviewPos;
}

void CompanyMenu::setMapPreviewScale(float newMapPreviewScale)
{
    mapPreviewScale = newMapPreviewScale;
}

void CompanyMenu::setLevelInfoPos(QPointF newLevelInfoPos)
{
    levelInfoPos = newLevelInfoPos;
}

void CompanyMenu::setLevelInfoFontSize(float newLevelInfoFontSize)
{
    levelInfoFontSize = newLevelInfoFontSize;
}

void CompanyMenu::setLevelListPos(QPointF newLevelListPos)
{
    levelListPos = newLevelListPos;
}

void CompanyMenu::setLevelListWidth(int newLevelListWidth)
{
    levelListWidth = newLevelListWidth;
}

const QString &CompanyMenu::getLevelChoice() const
{
    return levelChoice;
}

void CompanyMenu::loadXmlParameters()
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

        if (QString(node.attribute("name")) == "levels-list") {

            QDomNodeList menuSecondaryAttributes = node.childNodes();
            for (size_t j = 0, menuItemsNum = menuSecondaryAttributes.size(); j != menuItemsNum; j++) {
                auto currentElement = menuSecondaryAttributes.at(j).toElement();
                if(currentElement.tagName() == "attnum") {

                    if (QString(currentElement.attribute("name")).contains("font-size")) {

                        setLevelListFontSize(QString(currentElement.
                                                        attributes().
                                                        namedItem("val").
                                                        nodeValue()).
                                                toFloat());

                    } else if (QString(currentElement.attribute("name")).contains("interval")) {

                        setLevelListInterval(QString(currentElement.
                                                        attributes().
                                                        namedItem("val").
                                                        nodeValue()).
                                                toInt());

                    } else if (QString(currentElement.attribute("name")).contains("width")) {

                        setLevelListWidth(QString(currentElement.
                                                     attributes().
                                                     namedItem("val").
                                                     nodeValue()).
                                             toInt());


                    } else if (QString(currentElement.attribute("name")).contains("height")) {

                        // TODO : add scroll

                    }

                } else if (currentElement.tagName() == "attpos") {

                    // Set levels list coordinates
                    setLevelListPos(QPointF(QString(
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

        } else if (QString(node.attribute("name")) == "level-info") {

            QDomNodeList menuSecondaryAttributes = node.childNodes();
            for (size_t j = 0, menuItemsNum = menuSecondaryAttributes.size(); j != menuItemsNum; j++) {
                auto currentElement = menuSecondaryAttributes.at(j).toElement();

                if(currentElement.tagName() == "attnum") {

                    if (QString(currentElement.attribute("name")).contains("font-size")) {

                        setLevelInfoFontSize(QString(currentElement.
                                                        attributes().
                                                        namedItem("val").
                                                        nodeValue()).
                                                toFloat());

                    }

                } else if (currentElement.tagName() == "attpos") {

                    // Set level information coordinates
                    setLevelInfoPos(QPointF(QString(currentElement.
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

void CompanyMenu::processLevelClick()
{
    foreach (auto item, levels) {
        if (!item->isChosen())
        {
            continue;
        }

        foreach (auto subitem, levels) {
            subitem->setDefaultTextColor(Qt::black);
            subitem->getBackgroundRect()->setBrush(Qt::transparent);
        }

        item->setChosen(false);

        item->setDefaultTextColor(Qt::white);
        item->getBackgroundRect()->setBrush(QColor(153, 76, 0));

        levelChoice = item->getDirectoryName();

        mapPreview->setPixmap(QPixmap(LOCATIONS_DIRECTORY +
                                      item->getDirectoryName() +
                                      "/" +
                                      item->getLocationImage()));

        levelInfo->setPos(mapPreview->pos().x(),
                             mapPreview->pos().y() + mapPreview->boundingRect().height() * mapPreview->scale());

        if (preferences->getLanguage() == "English") {
            levelInfo->setPlainText("Name: " + item->getLocationFullName() + "\n"
                                       + "Vawes: " + QString::number(item->getWavesNum()));
        } else if (preferences->getLanguage() == "Українська") {
            levelInfo->setPlainText("Назва: " + item->getLocationFullName() + "\n"
                                       + "Хвилі: " + QString::number(item->getWavesNum()));
        } else if (preferences->getLanguage() == "Русский") {
            levelInfo->setPlainText("Название: " + item->getLocationFullName() + "\n"
                                       + "Волны: " + QString::number(item->getWavesNum()));
        }
    }
}
