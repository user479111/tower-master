#include "CompanyMenu.h"

#include <QStringList>
#include <QFileInfo>
#include <QDebug>
#include <QColor>

const QString CompanyMenu::XML_FILE_NAME = "CompanyMenu.xml";
const QString CompanyMenu::SAVE_COMPANY_FILE = "./Data/Company/SaveCompany.xml";

CompanyMenu::CompanyMenu(const Preferences *preferences)
  : Menu(preferences, XML_FILE_NAME),
    preferences(preferences),
    mapPreview(new QGraphicsPixmapItem()),
    info(new QGraphicsTextItem())
{
    loadXmlParameters();

    // do the preparation with all the XML parameters read
    prepare();
}

CompanyMenu::~CompanyMenu()
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

void CompanyMenu::prepare()
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
                MenuTextItem * menuTextItem = new MenuTextItem(levelInfo->getTitle(),
                                                               levelListWidth,
                                                               levelListFontSize,
                                                               QPointF(levelListPos.x(), levelListPos.y() +
                                                                       (levelListFontSize + levelListInterval) *
                                                                       levels.size()));
                menuTextItem->setActive(QString(node.attribute("active")).toInt());

                connect(menuTextItem, &MenuTextItem::clicked, this, &CompanyMenu::processLevelClick);

                levels.insert(levelInfo, menuTextItem);
            }
        }
        node = node.nextSibling().toElement();
    }
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
    scene->addItem(info);

    // Show locations list
    foreach (auto level, levels.values()) {
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
    scene->removeItem(info);

    // Hide locations
    for (auto level : levels.values()) {
        level->setChosen(false);
        scene->removeItem(level->getBackgroundRect());
        scene->removeItem(level);
    }

    // Clear location info
    info->setPlainText("");

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

void CompanyMenu::setInfoPos(QPointF newLevelInfoPos)
{
    infoPos = newLevelInfoPos;
}

void CompanyMenu::setInfoFontSize(float newLevelInfoFontSize)
{
    infoFontSize = newLevelInfoFontSize;
}

void CompanyMenu::setLevelListPos(QPointF newLevelListPos)
{
    levelListPos = newLevelListPos;
}

void CompanyMenu::setLevelListWidth(int newLevelListWidth)
{
    levelListWidth = newLevelListWidth;
}

const int CompanyMenu::getLevelChoiceId() const
{
    return levelChoiceId;
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

                        setInfoFontSize(QString(currentElement.
                                                attributes().
                                                namedItem("val").
                                                nodeValue()).
                                        toFloat());

                    } else if (QString(currentElement.attribute("name")).contains("width")) {

                        info->setTextWidth(QString(currentElement.
                                                   attributes().
                                                   namedItem("val").
                                                   nodeValue()).
                                           toInt());

                    }

                } else if (currentElement.tagName() == "attpos") {

                    // Set level information coordinates
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

void CompanyMenu::processLevelClick(MenuTextItem * Choice)
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
                                + "Vawes: " + QString::number(levelInfo->getWavesNum()) + "\n"
                                + "Description: " + QString(levelInfo->getTitle()));
    } else if (preferences->getLanguage() == "Українська") {
        info->setPlainText("Назва: " + levelInfo->getLocationName() + "\n"
                                + "Хвилі: " + QString::number(levelInfo->getWavesNum()) + "\n"
                                + "Опис: " + QString(levelInfo->getTitle()));
    } else if (preferences->getLanguage() == "Русский") {
        info->setPlainText("Название: " + levelInfo->getLocationName() + "\n"
                                + "Волны: " + QString::number(levelInfo->getWavesNum()) + "\n"
                                + "Описание: " + QString(levelInfo->getTitle()));
    }
}
