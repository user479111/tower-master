#include "Enemy.h"

#include <QtXml/QDomDocument>
#include <QFile>
#include <QtMath>

#include <QDebug>

Enemy::Enemy(int id,
             const QString &type,
             const QGraphicsPathItem * route) :
    id(id),
    type(type),
    route(route),
    speed(1),
    stepSize(MIN_ENEMY_SPEED),
    currentDestinationIndex(1),
    angle(0)
{
    // read Enemy.xml file and set MenuProcessor parameters
    loadXmlParameters(QString(":/Data/Data/Enemies/" + QString(type.at(0).toUpper() + type.mid(1)) + "/Enemy.xml"));

    setScale(route->scale());

    // Place the enemy at the beginning of the route
    setPos((route->path().elementAt(0) - boundingRect().bottomRight() / 2) * route->scale());
}

Enemy::~Enemy()
{
}

void Enemy::loadXmlParameters(QString inFileName)
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

        if (QString(node.tagName()) == "attstr") {

            if (QString(node.attribute("name")) == "skin") {
                qDebug() << "Skin:" << QString(":/Data/Data/Enemies/" +
                                               QString(type.at(0).toUpper() + type.mid(1)) + "/") <<
                                               QString(node.attribute("val"));
                setPixmap(QString(":/Data/Data/Enemies/" +
                                  QString(type.at(0).toUpper() + type.mid(1)) + "/") +
                                  QString(node.attribute("val")));
            }

        } else if (QString(node.tagName()) == "attnum") {

            if (QString(node.attribute("name")) == "health") {
                qDebug() << QString(node.attribute("val")).toInt();
            } else if (QString(node.attribute("name")) == "speed") {

                setSpeed(QString(node.attribute("val")).toInt());

            } else if (QString(node.attribute("name")) == "damage") {
                qDebug() << QString(node.attribute("val")).toInt();
            } else if (QString(node.attribute("name")) == "width") {
                qDebug() << QString(node.attribute("val")).toInt();
            } else if (QString(node.attribute("name")) == "height") {
                qDebug() << QString(node.attribute("val")).toInt();
            }
        }

        node = node.nextSibling().toElement();
    }
}

void Enemy::setRotation(qreal angle)
{
    this->angle = angle;

    setTransform(QTransform().
                 translate(pixmap().width()/2 * route->scale(), pixmap().height()/2 * route->scale()).
                 rotate(angle).
                 translate(-pixmap().width()/2 * route->scale(), -pixmap().height()/2 * route->scale()));

}

void Enemy::moveForward()
{
    // If close to dest rotate to next dest
    QLineF line(mapToScene(boundingRect().center()),
                route->path().elementAt(currentDestinationIndex) * route->scale());
    if (line.length() < stepSize) {

        if (++currentDestinationIndex == route->path().elementCount()) {
            prepareForRemoval();
            return;
        }

        line = QLineF(mapToScene(boundingRect().center()),
                      route->path().elementAt(currentDestinationIndex) * route->scale());

        setRotation(-1 * line.angle());
    }

    setPos(pos().x() + stepSize * cos(qDegreesToRadians(-1 * line.angle())),
           pos().y() + stepSize * sin(qDegreesToRadians(-1 * line.angle())));

    emit moved(this);
}

int Enemy::getSpeed() const
{
    return speed;
}

void Enemy::setSpeed(int newSpeed)
{
    if (newSpeed < MIN_ENEMY_SPEED) {
        speed = MIN_ENEMY_SPEED;
    } else {
        speed = newSpeed;
    }

    stepSize = speed * ENEMY_TIMER_INTERVAL / 1000;
}

void Enemy::setScale(qreal scale)
{
    setRotation(-angle);

    QGraphicsPixmapItem::setScale(this->scale() * scale);

    // Place enemy on the "scaled" position
    setPos(pos() * scale);

    setRotation(-angle);
}

void Enemy::prepareForRemoval()
{
    timerMove.stop();
    disconnect(&timerMove, SIGNAL(timeout()), this, SLOT(moveForward()));

    emit outOfBattleForMinimap(this); // Allow removal of the enemy from the Minimap
    emit outOfBattleForWave(this);    // Before it will be completely removed in the Wave
}

int Enemy::getId() const
{
    return id;
}

void Enemy::run()
{
    // run the enemy on the route
    setRotation(-1 * QLineF(mapToScene(boundingRect().center()),
                       route->path().elementAt(currentDestinationIndex) * route->scale()).angle());

    connect(&timerMove, SIGNAL(timeout()), this, SLOT(moveForward()));
    timerMove.start(ENEMY_TIMER_INTERVAL);
}

const QString &Enemy::getType() const
{
    return type;
}

void Enemy::setType(const QString &newType)
{
    type = newType;

    // read Enemy.xml file and set MenuProcessor parameters
    loadXmlParameters(QString(":/Data/Data/Enemies/" + QString(type.at(0).toUpper() + type.mid(1)) + "/Enemy.xml"));
}

