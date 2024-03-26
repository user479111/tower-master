#include "Enemy.h"

#include <QtXml/QDomDocument>
#include <QFile>
#include <QPen>
#include <QtMath>

#include <QDebug>

Enemy::Enemy(int id,
             const QString &type,
             const QGraphicsPathItem * route) :
    id(id),
    type(type),
    route(route),
    totalHealth(ENEMY_MIN_HEALTH),
    currentHealth(ENEMY_MIN_HEALTH),
    speed(ENEMY_MIN_SPEED),
    stepSize(ENEMY_MIN_SPEED),
    currentDestinationIndex(0),
    angle(0),
    moveTimer(this),
    timerRemainingTimeOnPause(0),
    totalHealthBar(new QGraphicsLineItem),
    currentHealthBar(new QGraphicsLineItem)
{
    // Read Enemy.xml file and set the parameters
    loadXmlParameters(QString(":/Data/Data/Enemies/" + QString(type.at(0).toUpper() + type.mid(1)) + "/Enemy.xml"));

    // Scale skin and health bars
    setScale(route->scale());
    totalHealthBar->setScale(route->scale());
    currentHealthBar->setScale(route->scale());

    // Place the enemy at the beginning of the route
    setPos(QPointF((route->path().elementAt(currentDestinationIndex++) -
                    boundingRect().bottomRight() / 2) * scale()));

    // Rotate to the next point on the route
    setRotation(-1 * QLineF(mapToScene(boundingRect().center()),
                       route->path().elementAt(currentDestinationIndex) * route->scale()).angle());
}

Enemy::~Enemy()
{
    delete totalHealthBar;
    delete currentHealthBar;
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

                // Set pixmap skin
                setPixmap(QString(":/Data/Data/Enemies/" +
                                  QString(type.at(0).toUpper() + type.mid(1)) + "/") +
                                  QString(node.attribute("val")));

                // Prepare total health bar
                totalHealthBar->setPen(QPen(QBrush(Qt::red), ENEMY_HEALTH_BAR_WIDTH));
                totalHealthBar->setLine(pos().x(),
                                        pos().y(),
                                        pos().x() + boundingRect().width() * scale(),
                                        pos().y());

                // Prepare current health bar
                currentHealthBar->setPen(QPen(QBrush(Qt::green), ENEMY_HEALTH_BAR_WIDTH));
                currentHealthBar->setLine(pos().x(),
                                          pos().y(),
                                          pos().x() + boundingRect().width() * scale(),
                                          pos().y());
            }

        } else if (QString(node.tagName()) == "attnum") {

            if (QString(node.attribute("name")) == "health") {

                setTotalHealth(QString(node.attribute("val")).toInt());
                setCurrentHealth(getTotalHealth());

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
                 translate(pixmap().width()/2 * scale(), pixmap().height()/2 * scale()).
                 rotate(angle).
                 translate(-pixmap().width()/2 * scale(), -pixmap().height()/2 * scale()));
}

void Enemy::moveForward()
{
    moveTimer.setInterval(ENEMY_TIMER_INTERVAL);

    // If close to dest rotate to next dest
    QLineF line(mapToScene(boundingRect().center()),
                route->path().elementAt(currentDestinationIndex) * route->scale());
    if (line.length() < stepSize * scale()) {

        if (++currentDestinationIndex == route->path().elementCount()) {
            prepareForRemoval();
            return;
        }

        line = QLineF(mapToScene(boundingRect().center()),
                      route->path().elementAt(currentDestinationIndex) * route->scale());

        setRotation(-1 * line.angle());
    }

    setPos(QPointF(pos().x() + stepSize * scale() * cos(qDegreesToRadians(-1 * line.angle())),
           pos().y() + stepSize * scale() * sin(qDegreesToRadians(-1 * line.angle()))));

    emit moved(this);
}

int Enemy::getCurrentHealth() const
{
    return currentHealth;
}

void Enemy::setCurrentHealth(int newCurrentHealth)
{
    currentHealth = (newCurrentHealth < ENEMY_MIN_HEALTH) ? ENEMY_MIN_HEALTH : newCurrentHealth;

    if(currentHealth < totalHealth) {
        // Decrease current health bar (Green line)
        currentHealthBar->setLine(0, 0, totalHealthBar->line().length() * currentHealth / totalHealth, 0);
    }
}

int Enemy::getTotalHealth() const
{
    return totalHealth;
}

void Enemy::setTotalHealth(int newTotalHealth)
{
    totalHealth = (newTotalHealth < ENEMY_MIN_HEALTH) ? ENEMY_MIN_HEALTH : newTotalHealth;
}

void Enemy::show(QGraphicsScene * scene)
{
    scene->addItem(this);
    scene->addItem(totalHealthBar);
    scene->addItem(currentHealthBar);
}

void Enemy::hide(QGraphicsScene * scene)
{
    scene->removeItem(this);
    scene->removeItem(totalHealthBar);
    scene->removeItem(currentHealthBar);
}

int Enemy::getSpeed() const
{
    return speed;
}

void Enemy::setSpeed(int newSpeed)
{
    speed = (newSpeed < ENEMY_MIN_SPEED) ? ENEMY_MIN_SPEED : newSpeed;

    stepSize = speed * ENEMY_TIMER_INTERVAL / 1000 * scale();
}

void Enemy::setScale(qreal scale)
{
    setRotation(-angle);

    QGraphicsPixmapItem::setScale(this->scale() * scale);

    totalHealthBar->setScale(totalHealthBar->scale() * scale);
    currentHealthBar->setScale(currentHealthBar->scale() * scale);


    // Place enemy on the "scaled" position
    setPos(pos() * scale);

    setRotation(-angle);
}

void Enemy::prepareForRemoval()
{
    moveTimer.stop();
    disconnect(&moveTimer, SIGNAL(timeout()), this, SLOT(moveForward()));

    emit outOfBattleForMinimap(this); // Allow removal of the enemy from the Minimap
    emit outOfBattleForWave(this);    // Before it will be completely removed in the Wave
}

void Enemy::setPos(const QPointF &pos)
{
    QGraphicsPixmapItem::setPos(pos);

    totalHealthBar->setPos(pos);
    currentHealthBar->setPos(pos);
}

int Enemy::getId() const
{
    return id;
}

void Enemy::run()
{
    // run the enemy on the route
    connect(&moveTimer, SIGNAL(timeout()), this, SLOT(moveForward()));
    moveTimer.start(ENEMY_TIMER_INTERVAL);
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

void Enemy::pause()
{
    if (moveTimer.isActive()) {
        timerRemainingTimeOnPause = moveTimer.remainingTime();
        moveTimer.stop();
    }
}

void Enemy::resume()
{
    if (timerRemainingTimeOnPause) {
        timerRemainingTimeOnPause = 0;
        moveTimer.start(timerRemainingTimeOnPause);
    }
}
