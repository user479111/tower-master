#include "Menu.h"

#include <QTextStream>
#include <QDebug>

const QString Menu::MENU_DIRECTORY = ":/Data/Data/Menu/";

Menu::Menu(const Preferences * preferences,
           const QString &xmlFileName,
           QObject * parent)
    : QObject{parent},
      preferences(preferences),
      backgroundImage(""),
      board(new QGraphicsPixmapItem()),
      boardImage(""),
      xmlFileName(xmlFileName)
{
    // read XML file and set the menu parameters
    loadXmlParameters();
}

Menu::~Menu()
{
    delete board;

    clearItemList();
    clearOptionList();
}

void Menu::loadXmlParameters()
{
    if (xmlFileName.isEmpty()) {
        return;
    }

    QDomDocument MenuProcessorXml;

    QFile xmlFile(MENU_DIRECTORY + xmlFileName);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error while loading file: " << MENU_DIRECTORY + xmlFileName;
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

        if (QString(node.attribute("name")) == "menu-content") {
            QDomNodeList menuItems = node.childNodes();

            for (size_t i = 0, menuItemsNum = menuItems.size(); i != menuItemsNum; i++) {
                auto menuItem = menuItems.at(i).toElement();

                if(QString(menuItem.tagName()) != "section") {
                    continue;
                }

                if (menuItem.attribute("type") == "menu-item") {

                    auto * newItem = new MenuItem();
                    readMenuItemXml(newItem, menuItem.childNodes(), menuItem.attribute("name"));
                    addNewItem(newItem);

                } else if (menuItem.attribute("type") == "scroll-option") {

                    auto * newOption = new ScrollOption();
                    readScrollOptionXml(newOption, menuItem.childNodes(), menuItem.attribute("name"));
                    addNewOption(newOption);

                }
            }

        } else if (QString(node.attribute("name")) == "background") {

            if (QString(node.firstChildElement().attribute("name")).contains("image")) {

                // Set background source image
                setBackgroundImage(QString(node.
                                           firstChildElement().
                                           attributes().
                                           namedItem("val").
                                           nodeValue()));

            }

        } else if (QString(node.attribute("name")) == "menu") {
            QDomNodeList menuSecondaryAttributes = node.childNodes();
            for (size_t j = 0, menuItemsNum = menuSecondaryAttributes.size(); j != menuItemsNum; j++) {

                auto currentElement = menuSecondaryAttributes.at(j).toElement();
                if(currentElement.tagName() == "attstr") {

                    if (QString(currentElement.attribute("name")).contains("image")) {

                        // Set menu board source image
                        setBoardImage(QString(currentElement.
                                              attributes().
                                              namedItem("val").
                                              nodeValue()));

                    }

                } else if (currentElement.tagName() == "attpos") {

                    // Set menu board coordinates
                    getBoard()->setPos(QPointF(QString(currentElement.
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

void Menu::readMenuItemXml(MenuItem * newItem, const QDomNodeList &menuItemAttributes, const QString &name)
{
    newItem->setTitle(name);

    // Set XML parameters for the item
    for (size_t k = 0, menuItemAttributesNum = menuItemAttributes.size(); k != menuItemAttributesNum; k++) {

        auto currentElement = menuItemAttributes.at(k).toElement();

        if(currentElement.tagName() == "attstr") {

            if (QString(currentElement.attribute("name")).contains("image")) {

                // Set item's background source image
                newItem->setPixmap(MENU_DIRECTORY +
                                   QString(currentElement.attributes().namedItem("val").nodeValue()));

            } else if (QString(currentElement.attribute("name")).contains("text") &&
                       (QString(currentElement.attribute("language")).contains(preferences->getLanguage()) ||
                        QString(currentElement.attribute("language")).contains("Any"))) {

                // Set text for the item
                newItem->setText(QString(currentElement.attributes().namedItem("val").nodeValue()));
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

        } else if (currentElement.tagName() == "attnum") {

            // Check if the item should be resizable
            if (QString(currentElement.attribute("name")).contains("fixed-size")) {
                newItem->setStaticSize(QString(currentElement.attributes().namedItem("val").nodeValue()) == "true");
            }

        }
    }

    connect(newItem, &MenuItem::clicked, this, &Menu::processItemClick);
}

void Menu::readScrollOptionXml(ScrollOption * newOption, const QDomNodeList &scrollOptionAttributes, const QString &name)
{
    newOption->setTitle(name);

    // Set XML parameters for the item
    for (size_t i = 0, scrollOptionAttributesNum = scrollOptionAttributes.size();
         i != scrollOptionAttributesNum; i++) {

        auto currentElement = scrollOptionAttributes.at(i).toElement();

        if(QString(currentElement.tagName()) == "section") {

            if (QString(currentElement.attribute("type")).contains("description")) {

                auto descriptionAttributes = currentElement.childNodes();
                for (size_t j = 0, descriptionAttributesNum = descriptionAttributes.size();
                     j != descriptionAttributesNum; j++) {

                    auto descriptionElement = descriptionAttributes.at(j).toElement();

                    if (QString(descriptionElement.attribute("name")).contains("text") &&
                        (QString(descriptionElement.attribute("language")).contains(preferences->getLanguage()) ||
                        QString(descriptionElement.attribute("language")).contains("Any"))) {

                        // Set description for the option
                        newOption->setDescription(QString(descriptionElement.
                                                          attributes().
                                                          namedItem("val").
                                                          nodeValue()));

                        // Ensure that arrows are the same size as the description
                        newOption->getLeftArrow()->setScale(newOption->getDescription()->boundingRect().height() /
                                                            newOption->getLeftArrow()->boundingRect().height());
                        newOption->getRightArrow()->setScale(newOption->getDescription()->boundingRect().height() /
                                                             newOption->getRightArrow()->boundingRect().height());

                    }
                }

            } else if (QString(currentElement.attribute("type")).contains("options")) {

                auto options = currentElement.childNodes();
                for (size_t j = 0, optionsNum = options.size(); j != optionsNum; j++) {
                    auto option = options.at(j).toElement();

                    if(QString(option.tagName()) == "section" &&
                       QString(option.attribute("type")).contains("option")) {

                        auto optionAttributes = option.childNodes();
                        for (size_t k = 0, optionAttributesNum = optionAttributes.size(); k != optionAttributesNum; k++) {
                            auto optionElement = optionAttributes.at(k).toElement();

                            if (QString(optionElement.attribute("name")).contains("text") &&
                                (QString(optionElement.attribute("language")).contains(preferences->getLanguage()) ||
                                QString(optionElement.attribute("language")).contains("Any"))) {

                                // Add option to the list of options
                                newOption->addOption(QString(optionElement.
                                                             attributes().
                                                             namedItem("val").
                                                             nodeValue()));
                            }
                        }
                    }
                }
            } else if (QString(currentElement.attribute("type")).contains("menu-item")) {

                MenuItem * currentItem = nullptr;

                if (QString(currentElement.attribute("name")).contains("left")) {
                    currentItem = newOption->getLeftArrow();
                } else if (QString(currentElement.attribute("name")).contains("right")) {
                    currentItem = newOption->getRightArrow();
                } else {

                    qDebug() << "Error: Wrong menu-item name for scroll-option.";
                    continue;

                }

                auto itemAttributes = currentElement.childNodes();
                for (size_t j = 0, itemAttributesNum = itemAttributes.size(); j != itemAttributesNum; j++) {
                    auto itemElement = itemAttributes.at(j).toElement();

                    if(itemElement.tagName() == "attstr") {

                        if (QString(itemElement.attribute("name")).contains("image")) {

                            // Set item's background source image
                            currentItem->setPixmap(MENU_DIRECTORY +
                                               QString(itemElement.
                                                       attributes().
                                                       namedItem("val").
                                                       nodeValue()));

                            // Ensure that the arrow is the same size as the description
                            currentItem->setScale(newOption->getDescription()->boundingRect().height() /
                                                  currentItem->boundingRect().height());

                        }

                    } else if (itemElement.tagName() == "attnum") {

                        // Check if the item should be resizable
                        if (QString(itemElement.attribute("name")).contains("fixed-size")) {
                            currentItem->setStaticSize(QString(itemElement.
                                                           attributes().
                                                           namedItem("val").
                                                           nodeValue()) == "true");
                        }
                    }
                }
            }

        } else if (currentElement.tagName() == "attpos") {

            // Set option's coordinates
            newOption->setPos(QPointF(QString(currentElement.
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

void Menu::clearItemList()
{
    for (auto item : listOfItems) {
        delete item;
    }
    listOfItems.clear();
}

void Menu::clearOptionList()
{
    for (auto option : listOfOptions) {
        delete option;
    }
    listOfOptions.clear();
}

void Menu::resetItemsLanguage()
{
    if (xmlFileName.isEmpty()) {
        return;
    }

    QDomDocument doc;
    QFile file(MENU_DIRECTORY + xmlFileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open XML file for reading: " << MENU_DIRECTORY + xmlFileName;
        return;
    }

    if (!doc.setContent(&file)) {
        qDebug() << "Failed to parse XML content: " << MENU_DIRECTORY + xmlFileName;
        file.close();
        return;
    }

    file.close();

    QDomElement root = doc.documentElement();
    QDomNodeList sectionList = root.elementsByTagName("section");

    // Update item list
    for (auto item : getListOfItems()) {

        for (size_t i = 0, sectionListNum = sectionList.size(); i < sectionListNum; ++i) {
            QDomElement section = sectionList.at(i).toElement();
            QString name = section.attribute("name");
            QString type = section.attribute("type");

            if (type == "menu-item" && name == item->getTitle() ) {

                QDomNodeList itemAttributes = section.childNodes();
                for (size_t j = 0, itemAttributesNum = itemAttributes.size(); j != itemAttributesNum; j++) {
                    auto itemAttribute = itemAttributes.at(j).toElement();

                    if(QString(itemAttribute.tagName()) == "attstr" &&
                       QString(itemAttribute.attribute("name")).contains("text") &&
                            (QString(itemAttribute.attribute("language")).contains(preferences->getLanguage()) ||
                             QString(itemAttribute.attribute("language")).contains("Any"))) {

                        // Set text for the item
                        item->setText(QString(itemAttribute.attributes().namedItem("val").nodeValue()));

                    }
                }
            }
        }
    }

    // Update option list
    clearOptionList();
    for (size_t i = 0, sectionListNum = sectionList.size(); i < sectionListNum; ++i) {
        QDomElement section = sectionList.at(i).toElement();
        QString name = section.attribute("name");
        QString type = section.attribute("type");

        if (type == "scroll-option") {
            auto * newOption = new ScrollOption();
            readScrollOptionXml(newOption, section.childNodes(), name);
            addNewOption(newOption);
        }
    }
}

const QString &Menu::getBackgroundImage() const
{
    return backgroundImage;
}

void Menu::setBackgroundImage(const QString &newBackgroundImage)
{
    backgroundImage = newBackgroundImage;
}

const QString &Menu::getBoardImage() const
{
    return boardImage;
}

void Menu::setBoardImage(const QString &newBoardImage)
{
    boardImage = newBoardImage;

    board->setPixmap(MENU_DIRECTORY + boardImage);
}

void Menu::addNewItem(MenuItem * newItem)
{
    listOfItems.append(newItem);
}

void Menu::addNewOption(ScrollOption *option)
{
    listOfOptions.append(option);
}

QGraphicsPixmapItem* Menu::getBoard() const
{
    return board;
}

const QList<MenuItem*> &Menu::getListOfItems() const
{
    return listOfItems;
}

const QList<ScrollOption *> &Menu::getListOfOptions() const
{
    return listOfOptions;
}

void Menu::processItemClick()
{
    emit itemClicked();
}

const QString Menu::transition()
{
    foreach (auto item, getListOfItems()) {

        if (!item->isChosen())
        {
            continue;
        }

        return item->getTitle();
    }

    return "";
}
