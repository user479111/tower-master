#include "Level.h"

#include <QFile>
#include <QDebug>
#include <QtXml/QDomDocument>

const QString Level::LEVEL_DIR_PREFIX = ":/Data/Data/Company/Levels/level_";
const QString Level::LEVEL_FILE_NAME = "Level.xml";

Level::Level(const Preferences * preferences,
             QGraphicsScene * scene,
             int id)
    : preferences(preferences),
      scene(scene),
      id(id),
      startMessage(""),
      victoryMessage("")
{
    loadXmlParameters();
}

Level::~Level()
{
    if (location) {
        delete location;
    }
}

int Level::getId() const
{
    return id;
}

Location *Level::getLocation() const
{
    return location;
}

void Level::setLocation(Location *newLocation)
{
    location = newLocation;
}

void Level::loadXmlParameters()
{
    {
        QDomDocument levelXml;

        QFile xmlFile(LEVEL_DIR_PREFIX + QString::number(id) + "/" + LEVEL_FILE_NAME);
        if (!xmlFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Error while loading file: " << LEVEL_DIR_PREFIX + id + "/" + LEVEL_FILE_NAME;
        }
        levelXml.setContent(&xmlFile);
        xmlFile.close();

        QDomElement root = levelXml.documentElement();
        QDomElement node = root.firstChild().toElement();

        // Read level parameters
        node = root.firstChild().toElement();
        while(!node.isNull())
        {
            if (QString(node.tagName()) == "section") {

                if (QString(node.attribute("name")) == "start-message") {
                    auto titleNodes = node.childNodes();
                    for (size_t i = 0, titleNodesNum = titleNodes.size(); i != titleNodesNum; i++) {
                        auto titleNode = titleNodes.at(i).toElement();

                        if (QString(titleNode.attribute("name")).contains("text") &&
                                (QString(titleNode.attribute("language")).contains(preferences->getLanguage()) ||
                                 QString(titleNode.attribute("language")).contains("Any"))) {

                            // Set text for the message
                            startMessage = QString(titleNode.attributes().namedItem("val").nodeValue());
                            break;
                        }
                    }
                } else if (QString(node.attribute("name")) == "victory-message") {
                    auto titleNodes = node.childNodes();
                    for (size_t i = 0, titleNodesNum = titleNodes.size(); i != titleNodesNum; i++) {
                        auto titleNode = titleNodes.at(i).toElement();

                        if (QString(titleNode.attribute("name")).contains("text") &&
                                (QString(titleNode.attribute("language")).contains(preferences->getLanguage()) ||
                                 QString(titleNode.attribute("language")).contains("Any"))) {

                            // Set text for the message
                            victoryMessage = QString(titleNode.attributes().namedItem("val").nodeValue());
                            break;
                        }
                    }
                }

            } else if (QString(node.tagName()) == "attstr") {
                QString name = QString(node.attribute("name"));

                if (name == "location") {
                    location = new Location(scene, QString(node.attribute("val")));
                }
            }
            node = node.nextSibling().toElement();
        }
    }
}

const QString &Level::getVictoryMessage() const
{
    return victoryMessage;
}

void Level::setVictoryMessage(const QString &newVictoryMessage)
{
    victoryMessage = newVictoryMessage;
}

const QString &Level::getStartMessage() const
{
    return startMessage;
}

void Level::setStartMessage(const QString &newStartMessage)
{
    startMessage = newStartMessage;
}
