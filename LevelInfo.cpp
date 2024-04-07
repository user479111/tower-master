#include "LevelInfo.h"

#include <QFile>
#include <QDebug>
#include <QtXml/QDomDocument>

const QString LevelInfo::LOCATIONS_DIRECTORY = ":/Data/Data/Locations/";
const QString LevelInfo::LOCATION_FILE_NAME = "Location.xml";
const QString LevelInfo::LEVEL_DIR_PREFIX = ":/Data/Data/Company/Levels/level_";
const QString LevelInfo::LEVEL_FILE_NAME = "Level.xml";

LevelInfo::LevelInfo(const Preferences * preferences,
                     int id)
    : preferences(preferences),
      id(id),
      locationDirName(""),
      locationName(""),
      locationImage(""),
      title(""),
      wavesNum(0)
{
    loadXmlParameters();
}

int LevelInfo::getId() const
{
    return id;
}

void LevelInfo::setId(int newId)
{
    id = newId;
}

const QString &LevelInfo::getLocationDirName() const
{
    return locationDirName;
}

void LevelInfo::setLocationDirName(const QString &newLocationDirName)
{
    locationDirName = newLocationDirName;
}

const QString &LevelInfo::getLocationName() const
{
    return locationName;
}

void LevelInfo::setLocationName(const QString &newLocationName)
{
    locationName = newLocationName;
}

const QString &LevelInfo::getLocationImage() const
{
    return locationImage;
}

void LevelInfo::setLocationImage(const QString &newLocationImage)
{
    locationImage = newLocationImage;
}

const QString &LevelInfo::getTitle() const
{
    return title;
}

void LevelInfo::setTitle(const QString &newTitle)
{
    title = newTitle;
}

int LevelInfo::getWavesNum() const
{
    return wavesNum;
}

void LevelInfo::setWavesNum(int newWavesNum)
{
    wavesNum = newWavesNum;
}

void LevelInfo::loadXmlParameters()
{
    // Read Level.xml
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
                    setTitle(QString(node.attribute("val")));
                } else if (name == "location") {
                    setLocationDirName(QString(node.attribute("val")));

                }
            }
            node = node.nextSibling().toElement();
        }
    }


    // Read Location.xml
    {
        QDomDocument locationXml;


        QFile xmlFile(LOCATIONS_DIRECTORY +
                      getLocationDirName() + "/" +
                      LOCATION_FILE_NAME);
        if (!xmlFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Error while loading file: " << LOCATIONS_DIRECTORY +
                        getLocationDirName() + "/" +
                        LOCATION_FILE_NAME;
        }

        locationXml.setContent(&xmlFile);
        xmlFile.close();

        QDomElement root = locationXml.documentElement();
        QDomElement node = root.firstChild().toElement();

        while(!node.isNull())
        {
            if(QString(node.tagName()) != "section") {
                node = node.nextSibling().toElement();
                continue;
            }

            if (QString(node.attribute("name")) == "main-info") {
                QDomNodeList locationAttributes = node.childNodes();

                for (size_t i = 0, menuAttributesNum = locationAttributes.size(); i != menuAttributesNum; i++) {

                    auto locationAttribute = locationAttributes.at(i).toElement();

                    if (QString(locationAttribute.tagName()) == "attstr") {

                        if (QString(locationAttribute.attribute("name")) == "name") {
                            setLocationName(QString(locationAttribute.attribute("val")));
                        } else if (QString(locationAttribute.attribute("name")) == "image") {
                            setLocationImage(LOCATIONS_DIRECTORY +
                                             getLocationDirName() + "/" +
                                             QString(locationAttribute.attribute("val")));
                        }

                    }

                }
            } else if (QString(node.attribute("name")) == "waves") {
                setWavesNum(node.childNodes().size());
            } else {
                // Do nothing for now
            }

            node = node.nextSibling().toElement();
        }
    }
}
