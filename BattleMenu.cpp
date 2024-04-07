#include "BattleMenu.h"

#include <QStringList>
#include <QDebug>
#include <QColor>

const QString BattleMenu::XML_FILE_NAME = "BattleMenu.xml";
const QString BattleMenu::LOCATIONS_DIRECTORY = ":/Data/Data/Locations/";
const QString BattleMenu::SAVE_COMPANY_FILE = "./Data/Company/SaveCompany.xml";

BattleMenu::BattleMenu(const Preferences * preferences)
    : Menu(preferences, XML_FILE_NAME),
      preferences(preferences),
      mapPreview(new QGraphicsPixmapItem()),
      info(new QGraphicsTextItem()),
      levelChoiceId(0)
{
    loadXmlParameters();

    // do the preparation with all the XML parameters read
    prepare();
}

BattleMenu::~BattleMenu()
{
    for(auto level: levels.keys()) {
        if (levels.value(level)) {
            delete levels.value(level);
        }
        delete level;
    }
    levels.clear();

    delete info;

    delete mapPreview;
}

void BattleMenu::prepare()
{
    mapPreview->setPos(mapPreviewPos);
    mapPreview->setScale(mapPreviewScale);

    info->setPos(mapPreview->pos().x(),
                         mapPreview->pos().y() + mapPreview->boundingRect().width() * mapPreview->scale());
    info->setDefaultTextColor(Qt::black);
    info->setFont(QFont("Helvetica [Cronyx]", infoFontSize, QFont::Medium));

    QDomDocument SaveCompanyXml;

    QFile xmlFile(SAVE_COMPANY_FILE);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error while loading file: " << SAVE_COMPANY_FILE;
    }
    SaveCompanyXml.setContent(&xmlFile);
    xmlFile.close();

    QDomElement root = SaveCompanyXml.documentElement();
    QDomElement node = root.firstChild().toElement();

    // Read company file
    node = root.firstChild().toElement();
    while(!node.isNull())
    {
        if (QString(node.tagName()) == "attnum") {
            QString name = QString(node.attribute("name"));
            if (name == "level") {
                LevelInfo * levelInfo = new LevelInfo(preferences,
                                                      QString(node.attribute("id")).toInt());
                MenuTextItem * menuTextItem = new MenuTextItem(levelInfo->getLocationName(),
                                                               levelListWidth,
                                                               levelListFontSize,
                                                               QPointF(levelListPos.x(), levelListPos.y() +
                                                                       (levelListFontSize + levelListInterval) *
                                                                       levels.size()));

                connect(menuTextItem, &MenuTextItem::clicked, this, &BattleMenu::processLevelClick);

                levels.insert(levelInfo, menuTextItem);
            }
        }
        node = node.nextSibling().toElement();
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
    mapPreview->setPixmap(QPixmap(MENU_DIRECTORY + mapPreviewImage));
    scene->addItem(mapPreview);

    // Display the information about location
    scene->addItem(info);

    // Show levels list
    foreach (auto item, levels.values()) {
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
    scene->removeItem(info);

    // Hide locations
    for (auto item : levels.values()) {
        item->setChosen(false);
        scene->removeItem(item->getBackgroundRect());
        scene->removeItem(item);
    }

    // Clear location info
    info->setPlainText("");

    // Hide items
    for (auto item : getListOfItems()) {
        item->hide(scene);
    }
}

void BattleMenu::processLevelClick(MenuTextItem * Choice)
{
    foreach (auto item, levels.values()) {
        if (item != Choice) {
            item->setChosen(false);
        }
    }

    auto levelInfo = levels.key(Choice);

    levelChoiceId = levelInfo->getId();

    mapPreview->setPixmap(QPixmap(levelInfo->getLocationImage()));

    info->setPos(mapPreview->pos().x(),
                         mapPreview->pos().y() + mapPreview->boundingRect().height() * mapPreview->scale());

    if (preferences->getLanguage() == "English") {
        info->setPlainText("Name: " + levelInfo->getLocationName() + "\n"
                                   + "Vawes: " + QString::number(levelInfo->getWavesNum()));
    } else if (preferences->getLanguage() == "Українська") {
        info->setPlainText("Назва: " + levelInfo->getLocationName() + "\n"
                                   + "Хвилі: " + QString::number(levelInfo->getWavesNum()));
    } else if (preferences->getLanguage() == "Русский") {
        info->setPlainText("Название: " + levelInfo->getLocationName() + "\n"
                                   + "Волны: " + QString::number(levelInfo->getWavesNum()));
    }
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

                    // Set locations list coordinates
                    setLevelListPos(QPointF(QString(currentElement.
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

                        setInfoFontSize(QString(currentElement.
                                                attributes().
                                                namedItem("val").
                                                nodeValue()).
                                        toFloat());

                    }

                } else if (currentElement.tagName() == "attpos") {

                    // Set location information coordinates
                    setInfoPos(QPointF(QString(currentElement.
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

void BattleMenu::setInfoWidth(int newInfoWidth)
{
    infoWidth = newInfoWidth;
}

void BattleMenu::setLevelListWidth(int newLevelListWidth)
{
    levelListWidth = newLevelListWidth;
}

const int BattleMenu::getLevelChoiceId() const
{
    return levelChoiceId;
}

void BattleMenu::setLevelListPos(QPointF newLevelListPos)
{
    levelListPos = newLevelListPos;
}

void BattleMenu::setLevelListInterval(int newLevelListInterval)
{
    levelListInterval = newLevelListInterval;
}

void BattleMenu::setInfoFontSize(float newInfoFontSize)
{
    infoFontSize = newInfoFontSize;
}

void BattleMenu::setInfoPos(QPointF newInfoPos)
{
    infoPos = newInfoPos;
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

void BattleMenu::setLevelListFontSize(int newLevelListFontSize)
{
    levelListFontSize = newLevelListFontSize;
}
