#include "CreditsMenu.h"

#include <QTextDocument>
#include <QTextOption>
#include <QDebug>

const QString CreditsMenu::XML_FILE_NAME = "CreditsMenu.xml";

const QString CreditsMenu::FONT_STYLE = "Helvetica [Cronyx]";
const int     CreditsMenu::FONT_SIZE  = 15;
const float   CreditsMenu::SPACE_FACTOR = 0.8;

CreditsMenu::CreditsMenu(Preferences * preferences)
    : Menu(preferences, XML_FILE_NAME),
      preferences(preferences)
{
    loadXmlParameters();
}

CreditsMenu::~CreditsMenu()
{
    for (auto credit : credits) {
        delete credit;
    }
    credits.clear();
}

void CreditsMenu::show(QGraphicsScene *scene)
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

    // Show items
    for (auto item : getListOfItems()) {
        item->setChosen(false);
        item->show(scene);
    }

    // Show credits
    for (auto credit : credits) {
        scene->addItem(credit);
    }
}

void CreditsMenu::hide(QGraphicsScene *scene)
{
    scene->removeItem(getBoard());

    // remove items
    for (auto item : getListOfItems()) {
        item->hide(scene);
    }

    // hide credits
    for (auto credit : credits) {
        scene->removeItem(credit);
    }
}

void CreditsMenu::loadXmlParameters()
{
    QDomDocument levelXml;

    QFile xmlFile(MENU_DIRECTORY + XML_FILE_NAME);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error while loading file: " << MENU_DIRECTORY + XML_FILE_NAME;
    }
    levelXml.setContent(&xmlFile);
    xmlFile.close();

    QDomElement root = levelXml.documentElement();
    QDomElement node = root.firstChild().toElement();

    // Read credits
    node = root.firstChild().toElement();
    while(!node.isNull())
    {
        if (QString(node.tagName()) == "section") {

            if (QString(node.attribute("name")) == "credits-content") {
                auto creditsNodes = node.childNodes();
                for (size_t i = 0, creditsNodesNum = creditsNodes.size(); i != creditsNodesNum; i++) {
                    auto creditsNode = creditsNodes.at(i).toElement();

                    if (QString(creditsNode.tagName()) == "section" &&
                            QString(creditsNode.attribute("name")).contains("row")) {
                        auto textNodes = creditsNode.childNodes();
                        for (size_t j = 0, textNodesNum = textNodes.size(); j != textNodesNum; j++) {
                            auto textNode = textNodes.at(j).toElement();
                            if (QString(textNode.attribute("name")).contains("text") &&
                                    (QString(textNode.attribute("language")).contains(preferences->getLanguage()) ||
                                     QString(textNode.attribute("language")).contains("Any"))) {

                                // Set text for credits
                                QGraphicsTextItem * credit = new QGraphicsTextItem();

                                credit->setTextWidth(getBoard()->boundingRect().width() * SPACE_FACTOR);
                                credit->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
                                credit->setDefaultTextColor(Qt::black);
                                credit->setFont(QFont(FONT_STYLE, FONT_SIZE, QFont::Bold));
                                credit->setZValue(1);
                                credit->setPlainText(QString(textNode.attributes().namedItem("val").nodeValue()));

                                credits.append(credit);
                                break;
                            }
                        }
                    }
                }
            }
            node = node.nextSibling().toElement();
        }
    }

    for (size_t i = 0, creditsNum = credits.size(); i < creditsNum; i++) {
        credits.at(i)->setPos(getBoard()->x() + getBoard()->boundingRect().width() / 2 -
                              credits.at(i)->boundingRect().width() / 2,
                              getBoard()->y() + getBoard()->boundingRect().height() / 2 -
                              credits.at(i)->boundingRect().height() * creditsNum / 2 +
                              i * credits.at(i)->boundingRect().height() + creditsNum / 2 -
                              credits.at(i)->boundingRect().height() / 2);
    }

}
