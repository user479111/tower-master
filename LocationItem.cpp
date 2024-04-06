#include "LocationItem.h"

#include <QtXml/QDomDocument>
#include <QFile>
#include <QFont>
#include <QPen>
#include <QDebug>

LocationItem::LocationItem(QString dyrectoryName,
                           int width,
                           int fontSize,
                           QPointF pos,
                           QGraphicsItem * parent) :
    QGraphicsTextItem(parent),
    description(""),
    chosen(false),
    active(true)
{
    directoryName = dyrectoryName;
    setPos(pos);

    setDefaultTextColor(Qt::black);
    setFont(QFont("Helvetica [Cronyx]", fontSize, QFont::Bold));

    backgroundRect = new QGraphicsRectItem(x(), y() + HIGHLIGHT_OFFSET, width, fontSize + HIGHLIGHT_OFFSET);
    backgroundRect->setBrush(Qt::transparent); // Set the fill color
    backgroundRect->setPen(QPen(Qt::transparent));

    loadXmlParameters(":/Data/Data/Locations/" + directoryName + "/Location.xml");

    setPlainText(directoryName);
}

LocationItem::~LocationItem()
{
    delete backgroundRect;
}

void LocationItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void LocationItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseReleaseEvent(event);

    if (active) {
        setChosen(true);
        emit clicked(this);
    }
}

const QString &LocationItem::getDirectoryName() const
{
    return directoryName;
}

void LocationItem::setDirectoryName(const QString &newDirectoryName)
{
    directoryName = newDirectoryName;
}

bool LocationItem::isChosen() const
{
    return chosen;
}

void LocationItem::setChosen(bool newChosen)
{
    chosen = newChosen;

    if (chosen) {

        setDefaultTextColor(Qt::white);
        backgroundRect->setBrush(QColor(153, 76, 0));

    } else {

        if (active) {
            setDefaultTextColor(Qt::black);
        } else {
            setDefaultTextColor(Qt::gray);
        }

        backgroundRect->setBrush(Qt::transparent);

    }
}

QGraphicsRectItem* LocationItem::getBackgroundRect() const
{
    return backgroundRect;
}

void LocationItem::setBackgroundRect(QGraphicsRectItem * newBackgroundRect)
{
    backgroundRect = newBackgroundRect;
}

void LocationItem::loadXmlParameters(QString inFileName)
{
    QDomDocument locationXml;

    QFile xmlFile(inFileName);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error while loading file";
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
                        locationFullName = QString(locationAttribute.attribute("val"));
                    } else if (QString(locationAttribute.attribute("name")) == "image") {
                        locationImage = QString(locationAttribute.attribute("val"));
                    }

                } else if (QString(locationAttribute.tagName()) == "attnum") {

                    if (QString(locationAttribute.attribute("name")) == "width") {
                        locationWidth = QString(locationAttribute.attribute("val")).toInt();
                    } else if (QString(locationAttribute.attribute("name")) == "height") {
                        locationHeight = QString(locationAttribute.attribute("val")).toInt();
                    }

                }

            }
        } else if (QString(node.attribute("name")) == "waves") {
            wavesNum = node.childNodes().size();
        } else {
            // Do nothing for now
        }

        node = node.nextSibling().toElement();
    }
}

const QString &LocationItem::getDescription() const
{
    return description;
}

void LocationItem::setDescription(const QString &newDescription)
{
    description = newDescription;

    setPlainText(description);
}

bool LocationItem::getActive() const
{
    return active;
}

void LocationItem::setActive(bool newActive)
{
    active = newActive;

    if (active) {
        setDefaultTextColor(Qt::black);
    } else {
        setDefaultTextColor(Qt::gray);
    }
}

int LocationItem::getWavesNum() const
{
    return wavesNum;
}

void LocationItem::setWavesNum(int newWavesNum)
{
    wavesNum = newWavesNum;
}

const QString &LocationItem::getLocationFullName() const
{
    return locationFullName;
}

void LocationItem::setLocationFullName(const QString &newLocationName)
{
    locationFullName = newLocationName;
}

const QString &LocationItem::getLocationImage() const
{
    return locationImage;
}

void LocationItem::setLocationImage(const QString &newLocationImage)
{
    locationImage = newLocationImage;
}
