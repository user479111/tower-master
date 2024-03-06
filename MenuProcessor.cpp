#include "MenuProcessor.h"

#include <QtXml/QDomDocument>
#include <QTextStream>
#include <QDebug>

MenuProcessor::MenuProcessor(QGraphicsScene * scene) :
    scene(scene),
    baseMenu(new BaseMenu()),
    battleMenu(new BattleMenu()),
    choice(StayInMenu)
{
    // read MenuProcessorXML file and set MenuProcessor parameters
    loadXmlParameters(QString(":/Data/Data/Menu/MenuProcessor.xml"));

    // do the preparation with all the XML parameters read
    battleMenu->prepare();

    // start with Base Menu
    currentMenu = baseMenu;

    // display menu
    currentMenu->show(scene);
}

MenuProcessor::~MenuProcessor()
{
    delete baseMenu;
    delete battleMenu;
}

void MenuProcessor::loadXmlParameters(QString inFileName)
{
    QDomDocument MenuProcessorXml;

    QFile xmlFile(inFileName);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error while loading file";
    }
    MenuProcessorXml.setContent(&xmlFile);
    xmlFile.close();

    auto root = MenuProcessorXml.documentElement();
    auto node = root.firstChild().toElement();

    Menu * processedMenu;

    while(!node.isNull())
    {
        if(QString(node.tagName()) != "section") {
            node = node.nextSibling().toElement();
            continue;
        }

        if (QString(node.attribute("name")) == "base-menu") {
            processedMenu = baseMenu;
        } else if (QString(node.attribute("name")) == "battle-menu") {
            processedMenu = battleMenu;
        } else {
            processedMenu = baseMenu;
        }

        auto menuAttributes = node.childNodes();

        for (size_t i = 0, menuAttributesNum = menuAttributes.size(); i != menuAttributesNum; i++) {

            auto menuAttribute = menuAttributes.at(i).toElement();

            if (QString(menuAttribute.tagName()) != "section") {
                continue;
            }

            if (QString(menuAttribute.attribute("name")) == "menu-content") {

                QDomNodeList menuItems = menuAttribute.childNodes();

                for (size_t j = 0, menuItemsNum = menuItems.size(); j != menuItemsNum; j++) {
                    auto menuItem = menuItems.at(j).toElement();

                    if(QString(menuItem.tagName()) != "section") {
                        continue;
                    }

                    QString menuItemName = menuItem.attribute("name");
                    if (menuItemName.isEmpty()) {
                        continue;
                    }

                    MenuItem * newItem = new MenuItem();

                    newItem->setTitle(menuItemName);

                    // Set XML parameters for the item
                    QDomNodeList menuItemAttributes = menuItem.childNodes();
                    for (size_t k = 0, menuItemAttributesNum = menuItemAttributes.size(); k != menuItemAttributesNum; k++) {

                        auto currentElement = menuItemAttributes.at(k).toElement();

                        if(currentElement.tagName() == "attstr") {

                            // Set item's source image
                            if (QString(currentElement.attribute("name")).contains("image")) {
                                newItem->setPixmap(":/Data/Data/Menu/" + QString(currentElement.attributes().namedItem("val").nodeValue()));
                            }

                        } else if (currentElement.tagName() == "attpos") {

                            // Set item's coordinates
                            newItem->setPos(QPointF(
                                                QString(
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


                    connect(newItem, &MenuItem::clicked, this, &MenuProcessor::processItemClick);

                    processedMenu->addNewItem(newItem);
                }

            } else if (QString(menuAttribute.attribute("name")) == "background") {

                if (QString(menuAttribute.firstChildElement().attribute("name")).contains("image")) {

                    // Set background source image
                    processedMenu->setBackgroundImage(
                                QString(menuAttribute.firstChildElement().attributes().namedItem("val").nodeValue()));

                }

            } else if (QString(menuAttribute.attribute("name")) == "menu") {

                QDomNodeList menuSecondaryAttributes = menuAttribute.childNodes();
                for (size_t j = 0, menuItemsNum = menuSecondaryAttributes.size(); j != menuItemsNum; j++) {

                    auto currentElement = menuSecondaryAttributes.at(j).toElement();
                    if(currentElement.tagName() == "attstr") {

                        if (QString(currentElement.attribute("name")).contains("image")) {

                            // Set menu board source image
                            processedMenu->setBoardImage(
                                        QString(currentElement.attributes().namedItem("val").nodeValue()));

                        }

                    } else if (currentElement.tagName() == "attpos") {

                        // Set menu board coordinates
                        processedMenu->getBoard()->setPos(QPointF(
                                                    QString(
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
            } else if (QString(menuAttribute.attribute("name")) == "locations-list") {

                QDomNodeList menuSecondaryAttributes = menuAttribute.childNodes();
                for (size_t j = 0, menuItemsNum = menuSecondaryAttributes.size(); j != menuItemsNum; j++) {
                    auto currentElement = menuSecondaryAttributes.at(j).toElement();
                    if(currentElement.tagName() == "attnum") {

                        if (QString(currentElement.attribute("name")).contains("font-size")) {

                            battleMenu->setLocationListFontSize(
                                        QString(currentElement.attributes().namedItem("val").nodeValue()).toFloat());

                        } else if (QString(currentElement.attribute("name")).contains("interval")) {

                            battleMenu->setLocationListInterval(
                                        QString(currentElement.attributes().namedItem("val").nodeValue()).toInt());

                        } else if (QString(currentElement.attribute("name")).contains("width")) {

                            battleMenu->setLocationListWidth(
                                        QString(currentElement.attributes().namedItem("val").nodeValue()).toInt());


                        } else if (QString(currentElement.attribute("name")).contains("height")) {

                            // TODO : add scroll

                        }

                    } else if (currentElement.tagName() == "attpos") {

                        // Set locations list coordinates
                        battleMenu->setLocationListPos(QPointF(
                                                    QString(
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

            } else if (QString(menuAttribute.attribute("name")) == "map-preview") {

                QDomNodeList menuSecondaryAttributes = menuAttribute.childNodes();
                for (size_t j = 0, menuItemsNum = menuSecondaryAttributes.size(); j != menuItemsNum; j++) {
                    auto currentElement = menuSecondaryAttributes.at(j).toElement();

                    if (currentElement.tagName() == "attstr") {

                        if (QString(currentElement.attribute("name")).contains("default-map")) {

                            // Set map preview source image
                            battleMenu->setMapPreviewImage(
                                        QString(currentElement.attributes().namedItem("val").nodeValue()));

                        }

                    } else if(currentElement.tagName() == "attnum") {

                        if (QString(currentElement.attribute("name")).contains("scale")) {

                            battleMenu->setMapPreviewScale(
                                        QString(currentElement.attributes().namedItem("val").nodeValue()).toFloat());

                        }

                    } else if (currentElement.tagName() == "attpos") {

                        // Set map preview coordinates
                        battleMenu->setMapPreviewPos(QPointF(
                                                    QString(
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

            } else if (QString(menuAttribute.attribute("name")) == "location-info") {

                QDomNodeList menuSecondaryAttributes = menuAttribute.childNodes();
                for (size_t j = 0, menuItemsNum = menuSecondaryAttributes.size(); j != menuItemsNum; j++) {
                    auto currentElement = menuSecondaryAttributes.at(j).toElement();

                    if(currentElement.tagName() == "attnum") {

                        if (QString(currentElement.attribute("name")).contains("font-size")) {

                            battleMenu->setLocationInfoFontSize(
                                        QString(currentElement.attributes().namedItem("val").nodeValue()).toFloat());

                        }

                    } else if (currentElement.tagName() == "attpos") {

                        // Set location information coordinates
                        battleMenu->setLocationInfoPos(QPointF(
                                                    QString(
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

            }
        }
        node = node.nextSibling().toElement();
    }
}

void MenuProcessor::processItemClick()
{
    if (currentMenu->transition().isEmpty()) {
        return;
    }

    currentMenu->hide(scene);

    if (dynamic_cast<BaseMenu*>(currentMenu)) {

        if (currentMenu->transition() == "company") {
            // Process CompanyMenu class.
        } else if (currentMenu->transition() == "battle") {
            currentMenu = battleMenu;
        } else if (currentMenu->transition() == "settings") {
            // Process SettingsMenu class.
        } else if (currentMenu->transition() == "credits") {
            // Process CreditsMenu class.
        } else if (currentMenu->transition() == "quit") {

            choice = Quit;
            emit keyChoiseMade();

        } else {
            qDebug() << "Incorrect transition: " << currentMenu->transition();
        }

    } else if (dynamic_cast<BattleMenu*>(currentMenu)) {

        if (currentMenu->transition() == "back") {
            currentMenu = baseMenu;
        } else if (currentMenu->transition() == "load") {

            if (!(locationChoice = battleMenu->getLocationChoice()).isEmpty()) {
                choice = StartGame;
                emit  keyChoiseMade();
            }
        }

    } else {
        qDebug() << "Incorrect type";
    }

    if (choice == StayInMenu) {
        currentMenu->show(scene);
    }
}

const QString &MenuProcessor::getLocationChoice() const
{
    return locationChoice;
}

MenuProcessor::Choice MenuProcessor::getChoice() const
{
    return choice;
}
