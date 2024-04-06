#include "CompanyMenu.h"

#include <QStringList>
#include <QFileInfo>
#include <QDebug>
#include <QColor>

const QString CompanyMenu::XML_FILE_NAME = "CompanyMenu.xml";
const QString CompanyMenu::SAVE_COMPANY_FILE = "./Data/Company/SaveCompany.xml";
const QString CompanyMenu::LOCATIONS_DIRECTORY = ":/Data/Data/Locations/";
const QString CompanyMenu::LEVEL_DIR_PREFIX = ":/Data/Data/Company/Levels/level_";
const QString CompanyMenu::LEVEL_FILE_NAME = "Level.xml";

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
    mapPreview->setPos(mapPreviewPos);
    mapPreview->setScale(mapPreviewScale);

    levelInfo->setPos(mapPreview->pos().x(),
                         mapPreview->pos().y() + mapPreview->boundingRect().width() * mapPreview->scale());
    levelInfo->setDefaultTextColor(Qt::black);
    levelInfo->setFont(QFont("Helvetica [Cronyx]", levelInfoFontSize, QFont::Medium));

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
                prepareLevel(QString(node.attribute("id")),
                             QString(node.attribute("active")).toInt());
            }
        }
        node = node.nextSibling().toElement();
    }
}

void CompanyMenu::prepareLevel(const QString &id, bool active)
{
    QDomDocument levelXml;

    QFile xmlFile(LEVEL_DIR_PREFIX + id + "/" + LEVEL_FILE_NAME);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error while loading file: " << LEVEL_DIR_PREFIX + id + "/" + LEVEL_FILE_NAME;
    }
    levelXml.setContent(&xmlFile);
    xmlFile.close();

    QDomElement root = levelXml.documentElement();
    QDomElement node = root.firstChild().toElement();

    LocationItem * level = nullptr;
    QString title = "", location = "";
    // Read level parameters
    node = root.firstChild().toElement();
    while(!node.isNull())
    {
        if (QString(node.tagName()) == "section") {

            if (QString(node.attribute("name")) == "title") {
                auto titleNodes = node.childNodes();
                for (size_t i = 0, titleNodesNum = titleNodes.size(); i != titleNodesNum; i++) {
                    auto titleNode = titleNodes.at(i).toElement();

                    if (QString(titleNode.attribute("name")).contains("text") &&
                            (QString(titleNode.attribute("language")).contains(preferences->getLanguage()) ||
                             QString(titleNode.attribute("language")).contains("Any"))) {

                        // Set text for the title
                        title = QString(titleNode.attributes().namedItem("val").nodeValue());
                        break;
                    }
                }
            }

        } else if (QString(node.tagName()) == "attstr") {
            QString name = QString(node.attribute("name"));
            if (name == "title") {
                title = QString(node.attribute("val"));
            } else if (name == "location") {
                location = QString(node.attribute("val"));

            }
        }
        node = node.nextSibling().toElement();
    }

    level = new LocationItem(location, levelListWidth, levelListFontSize,
                             QPointF(levelListPos.x(), levelListPos.y() +
                                     (levelListFontSize + levelListInterval) *
                                     levels.size())); // Each new item should be lower that previous

    level->setDescription(title);
    level->setActive(active);

    connect(level, &LocationItem::clicked, this, &CompanyMenu::processLevelClick);

    levels.append(level);

    levelInfo->setTextWidth(levelInfoWidth);
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
    mapPreview->setPixmap(QPixmap(MENU_DIRECTORY + mapPreviewImage));
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
        level->setChosen(false);
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
    QDomDocument CompanyMenuXml;

    QFile xmlFile(MENU_DIRECTORY + XML_FILE_NAME);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error while loading file: " << MENU_DIRECTORY + XML_FILE_NAME;
    }
    CompanyMenuXml.setContent(&xmlFile);
    xmlFile.close();

    auto root = CompanyMenuXml.documentElement();
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

                    } else if (QString(currentElement.attribute("name")).contains("width")) {

                        setLevelInfoWidth(QString(currentElement.
                                                        attributes().
                                                        namedItem("val").
                                                        nodeValue()).
                                                toInt());

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

void CompanyMenu::processLevelClick(LocationItem * choise)
{
    foreach (auto item, levels) {
        if (item != choise) {
            item->setChosen(false);
        }
    }

    levelChoice = choise->getDirectoryName();

    mapPreview->setPixmap(QPixmap(LOCATIONS_DIRECTORY +
                                  choise->getDirectoryName() +
                                  "/" +
                                  choise->getLocationImage()));

    levelInfo->setPos(mapPreview->pos().x(),
                         mapPreview->pos().y() + mapPreview->boundingRect().height() * mapPreview->scale());

    if (preferences->getLanguage() == "English") {
        levelInfo->setPlainText("Name: " + choise->getLocationFullName() + "\n"
                                + "Vawes: " + QString::number(choise->getWavesNum()) + "\n"
                                + "Description: " + QString(choise->getDescription()));
    } else if (preferences->getLanguage() == "Українська") {
        levelInfo->setPlainText("Назва: " + choise->getLocationFullName() + "\n"
                                + "Хвилі: " + QString::number(choise->getWavesNum()) + "\n"
                                + "Опис: " + QString(choise->getDescription()));
    } else if (preferences->getLanguage() == "Русский") {
        levelInfo->setPlainText("Название: " + choise->getLocationFullName() + "\n"
                                + "Волны: " + QString::number(choise->getWavesNum()) + "\n"
                                + "Описание: " + QString(choise->getDescription()));
    }
}

void CompanyMenu::setLevelInfoWidth(int newLevelInfoWidth)
{
    levelInfoWidth = newLevelInfoWidth;
}
