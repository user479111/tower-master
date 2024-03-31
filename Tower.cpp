#include "Tower.h"
#include "Enemy.h"
#include "Bullet.h"

#include <QGraphicsScene>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QtMath>
#include <QDebug>
#include <QTimer>

Tower::Tower(const QString &directoryType,
             const QPointF &center,
             const Location * location) :
    location(location),
    attackTimer(this),
    skin(""),
    type(""),
    directoryType(directoryType),
    attackSpeed(MIN_ATTACK_SPEED)
{
    // read Enemy.xml file and set MenuProcessor parameters
    loadXmlParameters(QString(":/Data/Data/Towers/" + directoryType + "/Tower.xml"));

    setScale(location->scale());

    // Update position to fit the scale
    setPos(center.x() - pixmap().width() * location->scale() / 2,
           center.y() - pixmap().height() * location->scale() / 2);

    // create attack area for tower
    int AREA_RADIUS = 200;
    int POLYGON_NUMBER = 12;

    QPolygonF polygon;
    float angle = 360 / POLYGON_NUMBER;

    // calculate corners' coordinates
    for (int i = 0; i < POLYGON_NUMBER; i++) {
        polygon.append(QPointF(boundingRect().center().x() + AREA_RADIUS * cos(qDegreesToRadians(angle * i)),
                               boundingRect().center().y() + AREA_RADIUS * sin(qDegreesToRadians(angle * i))));
    }

    attackArea = new QGraphicsPolygonItem(polygon, this);
    attackArea->setPen(QPen(Qt::DotLine));
    attackArea->setPen(QPen(QColor(255, 0, 0)));

    // Prepare tower for attack
    connect(&attackTimer, SIGNAL(timeout()), this, SLOT(aquireTarget()));
    // Prepare tower to clear the bullets
    connect(&attackTimer, SIGNAL(timeout()), this, SLOT(clearBullets()));
    attackTimer.start(1000/attackSpeed);

    highlightedObjectId = getId();
}

Tower::~Tower()
{
    qDebug() << "Tower::~Tower";
    delete attackArea;

    for (auto bullet : bullets) {
        delete bullet;
    }
    bullets.clear();
}

void Tower::fire()
{
    Bullet * bullet = new Bullet(bulletPatern);

    // Display the bullet
    bullet->show(scene());
    bullet->setScale(scale());
    bullet->setPos(mapToScene(boundingRect().center()));

    // Set the angle for the bullet movement
    QLineF attackLine(mapToScene(boundingRect().center()), attackDestination);
    bullet->setAngle(-1 * attackLine.angle());

    // Prepare for the bullet removal on the target reaching event
    connect(bullet, &Bullet::targetReached, this, &Tower::clearBullets);

    // Start movement
    bullet->shot();

    // Add the bullet to the list of tower's bullets
    bullets.append(bullet);
}

void Tower::aquireTarget()
{
    attackTimer.setInterval(1000/attackSpeed);

    // get a list of items colliding with attack_area
    auto collidingItems = attackArea->collidingItems();

    // is there something in the area (except the tower)
    if (collidingItems.size() == 1) {
        return;
    }

    // find closest enemy
    float closest_distance = std::numeric_limits<float>::max();
    for (auto collidingItem : collidingItems) {
        auto enemy = dynamic_cast<Enemy *>(collidingItem);

        if (!enemy) continue;

        float curent_distance = QLineF(mapToScene(boundingRect().center()),
                                       mapToScene(mapFromItem(enemy,
                                                              enemy->boundingRect().center()))).length();

        if (curent_distance >= closest_distance) continue;

        closest_distance = curent_distance;
        attackDestination = mapToScene(mapFromItem(enemy, enemy->boundingRect().center()));
    }

    if (closest_distance < std::numeric_limits<float>::max()) {
        fire();
    }
}

// If bullet is marked as "outOfBattle"
// remove it from scene and from the bullets list
void Tower::clearBullets()
{
    if (bullets.size() > 0) {
        for (auto bulletItem : bullets) {
            if (bulletItem->isOutOfBattle()) {
                bulletItem->hide(scene());
                bullets.removeOne(bulletItem);
                delete bulletItem;
            }
        }
    }
}

const QString &Tower::getSkin() const
{
    return skin;
}

void Tower::setAttackSpeed(float newAttackSpeed)
{
    if (newAttackSpeed < MIN_ATTACK_SPEED) {
        attackSpeed = MIN_ATTACK_SPEED;
    } else if (newAttackSpeed > MAX_ATTACK_SPEED) {
        attackSpeed = MAX_ATTACK_SPEED;
    } else {
        attackSpeed = newAttackSpeed;
    }
}

void Tower::pause()
{
    if (attackTimer.isActive()) {
        timerRemainingTimeOnPause = attackTimer.remainingTime();
        attackTimer.stop();
    }

    for (auto bullet : bullets) {
        bullet->pause();
    }
}

void Tower::resume()
{
    if (timerRemainingTimeOnPause) {
        timerRemainingTimeOnPause = 0;
        attackTimer.start(timerRemainingTimeOnPause);
    }

    for (auto bullet : bullets) {
        bullet->resume();
    }
}


const QList<Bullet*> &Tower::getBullets() const
{
    return bullets;
}

int Tower::getDamage() const
{
    return bulletPatern.getDamage();
}

float Tower::getAttackSpeed() const
{
    return attackSpeed;
}

void Tower::loadXmlParameters(QString inFileName)
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

        if (QString(node.attribute("name")) == "tower") {
            QDomNodeList towerAttributes = node.childNodes();

            for (size_t i = 0, towerAttributesNum = towerAttributes.size(); i != towerAttributesNum; i++) {

                auto towerAttribute = towerAttributes.at(i).toElement();

                if (QString(towerAttribute.tagName()) == "attstr") {

                    if (QString(towerAttribute.attribute("name")) == "type") {

                        type = QString(towerAttribute.attribute("val"));

                    } else if (QString(towerAttribute.attribute("name")) == "skin") {

                        skin = QString(":/Data/Data/Towers/" + directoryType + "/" + QString(towerAttribute.attribute("val")));
                        setPixmap(skin);

                    }

                } else if (QString(towerAttribute.tagName()) == "attnum") {

                    if (QString(towerAttribute.attribute("name")) == "attack-speed") {

                        qDebug() << "Tower attack-speed: " << QString(towerAttribute.attribute("val")).toInt();

                        setAttackSpeed(QString(towerAttribute.attribute("val")).toInt());

                    } else if (QString(towerAttribute.attribute("name")) == "width") {
                        qDebug() << "Tower width: " << QString(towerAttribute.attribute("val")).toInt();
                    } else if (QString(towerAttribute.attribute("name")) == "height") {
                        qDebug() << "Tower height: " << QString(towerAttribute.attribute("val")).toInt();
                    }
                }
            }
        } else if (QString(node.attribute("name")) == "bullet") {
            QDomNodeList bulletAttributes = node.childNodes();

            bulletPatern.setLocation(location);

            for (size_t i = 0, bulletAttributesNum = bulletAttributes.size(); i != bulletAttributesNum; i++) {

                auto bulletAttribute = bulletAttributes.at(i).toElement();

                if (QString(bulletAttribute.tagName()) == "attstr") {

                    if (QString(bulletAttribute.attribute("name")) == "skin") {
                        bulletPatern.setPixmap(QString(":/Data/Data/Towers/" + directoryType + "/" + QString(bulletAttribute.attribute("val"))));
                    }

                } else if (QString(bulletAttribute.tagName()) == "attnum") {

                    if (QString(bulletAttribute.attribute("name")) == "damage") {
                        bulletPatern.setDamage(QString(bulletAttribute.attribute("val")).toInt());
                        qDebug() << "Bullet damage: " << QString(bulletAttribute.attribute("val")).toInt();
                    } else if (QString(bulletAttribute.attribute("name")) == "speed") {
                        bulletPatern.setSpeed(QString(bulletAttribute.attribute("val")).toInt());
                        qDebug() << "Bullet speed: " << QString(bulletAttribute.attribute("val")).toInt();

                        //setAttackSpeed(QString(towerAttribute.attribute("val")).toInt());

                    } else if (QString(bulletAttribute.attribute("name")) == "width") {
                        qDebug() << "Bullet width: " << QString(bulletAttribute.attribute("val")).toInt();
                    } else if (QString(bulletAttribute.attribute("name")) == "height") {
                        qDebug() << "Bullet height: " << QString(bulletAttribute.attribute("val")).toInt();
                    }
                }
            }
        }


        node = node.nextSibling().toElement();
    }
}

const QString &Tower::getType() const
{
    return type;
}
