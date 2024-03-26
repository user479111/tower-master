#include "Location.h"
#include "Enemy.h"

#include <QGraphicsScene>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QDebug>

Location::Location(QGraphicsScene * scene, QString name) :
    scene(scene),
    name(name),
    locationImage(""),
    locationImagePath(""),
    width(0),
    height(0),
    timeForPreparation(0),
    enemyDamageGoal(0)
{
    // read Location.xml file and set Location parameters
    loadXmlParameters(QString(":/Data/Data/Locations/" + name + "/Location.xml"));

    locationImagePath = ":/Data/Data/Locations/" + name + "/" + locationImage;

    setPixmap(locationImagePath);
}

Location::~Location()
{
    for (auto buildArea : buildAreas) {
        delete buildArea;
    }
    buildAreas.clear();

    for (auto enemyRoute : enemyRoutes) {
        delete enemyRoute;
    }
    enemyRoutes.clear();

    for (auto wave : waves) {
        delete wave;
    }
    waves.clear();
}

void Location::loadXmlParameters(QString inFileName)
{
    QDomDocument MenuProcessorXml;

    QFile xmlFile(inFileName);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error while loading file";
    }
    MenuProcessorXml.setContent(&xmlFile);
    xmlFile.close();

    QDomElement root = MenuProcessorXml.documentElement();
    QDomElement node = root.firstChild().toElement();

    while(!node.isNull())
    {
        if(QString(node.tagName()) != "section") {
            node = node.nextSibling().toElement();
            continue;
        }

        if (QString(node.attribute("name")) == "main-info") {
            QDomNodeList locationAttributes = node.childNodes();

            for (size_t i = 0, locationAttributesNum = locationAttributes.size(); i != locationAttributesNum; i++) {

                auto locationAttribute = locationAttributes.at(i).toElement();

                if (QString(locationAttribute.tagName()) == "attstr") {

                    if (QString(locationAttribute.attribute("name")) == "image") {
                        locationImage = QString(locationAttribute.attribute("val"));
                    }

                } else if (QString(locationAttribute.tagName()) == "attnum") {

                    if (QString(locationAttribute.attribute("name")) == "width") {
                        width = QString(locationAttribute.attribute("val")).toInt();
                    } else if (QString(locationAttribute.attribute("name")) == "height") {
                        height = QString(locationAttribute.attribute("val")).toInt();
                    } else if (QString(locationAttribute.attribute("name")) == "time-for-preparation") {
                        timeForPreparation = QString(locationAttribute.attribute("val")).toInt();
                    } else if (QString(locationAttribute.attribute("name")) == "enemy-damage-goal") {
                        enemyDamageGoal = QString(locationAttribute.attribute("val")).toInt();
                    }

                }

            }
        } else if (QString(node.attribute("name")) == "possible-build-positions") {

            auto polygonXmlNodes = node.childNodes();

            for (size_t i = 0, polygonXmlNodesNum = polygonXmlNodes.size(); i != polygonXmlNodesNum; i++) {

                auto polygonXmlNode = polygonXmlNodes.at(i).toElement();

                if (QString(polygonXmlNode.attribute("name")) != "polygon") {
                    continue;
                }

                QPolygonF buildAreaPolygon;

                auto poligonAttributes = polygonXmlNode.childNodes();

                for (size_t j = 0, poligonAttributesNum = poligonAttributes.size(); j != poligonAttributesNum; j++) {

                    auto polygonAttribute = poligonAttributes.at(j).toElement();

                    if (polygonAttribute.tagName() == "attpos") {

                        // Set item's coordinates
                        buildAreaPolygon.append(QPointF(
                                            QString(
                                                polygonAttribute.
                                                attributes().
                                                namedItem("x").
                                                nodeValue()).
                                                toFloat(),
                                            QString(
                                                polygonAttribute.
                                                attributes().
                                                namedItem("y").
                                                nodeValue()).
                                                toFloat()));

                    }
                }

                QGraphicsPolygonItem * newBuildArea(new QGraphicsPolygonItem(buildAreaPolygon));

                buildAreas.append(newBuildArea);

            }
        } else if (QString(node.attribute("name")) == "enemy-routes") {

            auto routesXmlNodes = node.childNodes();

            for (size_t i = 0, routesXmlNodesNum = routesXmlNodes.size(); i != routesXmlNodesNum; i++) {

                auto routeXmlNode = routesXmlNodes.at(i).toElement();

                if (QString(routeXmlNode.attribute("name")) != "route") {
                    continue;
                }

                QPolygon enemyRoute;

                auto poligonAttributes = routeXmlNode.childNodes();

                for (size_t j = 0, poligonAttributesNum = poligonAttributes.size(); j != poligonAttributesNum; j++) {

                    auto polygonAttribute = poligonAttributes.at(j).toElement();

                    if (polygonAttribute.tagName() == "attpos") {

                        // Set item's coordinates
                        enemyRoute.append(QPoint(
                                            QString(
                                                polygonAttribute.
                                                attributes().
                                                namedItem("x").
                                                nodeValue()).
                                                toFloat(),
                                            QString(
                                                polygonAttribute.
                                                attributes().
                                                namedItem("y").
                                                nodeValue()).
                                                toFloat()));

                    }
                }

                // Create new enemy route of the QGraphicsPathItem type
                QPainterPath newPath;
                newPath.addPolygon(enemyRoute);
                QGraphicsPathItem * newEnemyRoute(new QGraphicsPathItem(newPath));

                // Add to the list
                enemyRoutes.append(newEnemyRoute);

            }
        } else if (QString(node.attribute("name")) == "waves") {

            auto wavesXmlNodes = node.childNodes();

            for (size_t i = 0, wavesXmlNodesNum = wavesXmlNodes.size(); i != wavesXmlNodesNum; i++) {

                auto waveXmlNode = wavesXmlNodes.at(i).toElement();

                if (QString(waveXmlNode.attribute("name")) != "wave") {
                    continue;
                }

                Wave * newWave(new Wave(scene));

                newWave->setId(QString(waveXmlNode.attributes().namedItem("id").nodeValue()).toInt());

                auto waveElements = waveXmlNode.childNodes();

                for (size_t j = 0, waveElementsNum = waveElements.size(); j != waveElementsNum; j++) {

                    auto waveElement = waveElements.at(j).toElement();

                    if (waveElement.tagName() == "attnum") {

                        if (QString(waveElement.attribute("name")) == "interval") {
                            newWave->setInterval(QString(waveElement.attributes().namedItem("val").nodeValue()).toInt());
                        } else if (QString(waveElement.attribute("name")) == "dencity") {
                            newWave->setDencity(QString(waveElement.attributes().namedItem("val").nodeValue()).toFloat());
                        }

                    }
                }

                waves.append(newWave);
            }

        } /*else {
        }*/

        node = node.nextSibling().toElement();
    }
}

int Location::getEnemyDamageGoal() const
{
    return enemyDamageGoal;
}

const QString &Location::getName() const
{
    return name;
}

const QList<Wave*> &Location::getWaves() const
{
    return waves;
}

short Location::getTimeForPreparation() const
{
    return timeForPreparation;
}

const QList<QGraphicsPathItem*> &Location::getEnemyRoutes() const
{
    return enemyRoutes;
}

const QList<QGraphicsPolygonItem*> &Location::getBuildAreas() const
{
    return buildAreas;
}

const QString &Location::getLocationImagePath() const
{
    return locationImagePath;
}

int Location::getHeight() const
{
    return height;
}

int Location::getWidth() const
{
    return width;
}
