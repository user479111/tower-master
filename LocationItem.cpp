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
                           QSharedPointer<QGraphicsItem> parent) :
    QGraphicsTextItem(parent.get())
{
    directoryName = dyrectoryName;
    setPos(pos);

    setDefaultTextColor(Qt::black);
    setFont(QFont("Helvetica [Cronyx]", fontSize, QFont::Bold));

    backgroundRect = QSharedPointer<QGraphicsRectItem>
            (new QGraphicsRectItem(x(), y() + HIGHLIGHT_OFFSET, width, fontSize + HIGHLIGHT_OFFSET));
    backgroundRect->setBrush(Qt::transparent); // Set the fill color
    backgroundRect->setPen(QPen(Qt::transparent));

    loadXmlParameters(":/Data/Data/Locations/" + directoryName + "/Location.xml");

    setPlainText(directoryName);
}

void LocationItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void LocationItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseReleaseEvent(event);

    setChosen(true);

    emit clicked();
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
}

QSharedPointer<QGraphicsRectItem> LocationItem::getBackgroundRect() const
{
    return backgroundRect;
}

void LocationItem::setBackgroundRect(QSharedPointer<QGraphicsRectItem> newBackgroundRect)
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
