#include "Wave.h"
#include "Location.h"

#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>

Wave::Wave(const QSharedPointer<QGraphicsScene> scene):
    interval(0),
    dencity(1),
    currentEnemyId(0),
    enemiesOutOfBattleNum(0),
    scene(scene)
{
}

Wave::~Wave()
{
}

int Wave::getInterval() const
{
    return interval;
}

void Wave::setInterval(int newInterval)
{
    interval = newInterval;
}

float Wave::getDencity() const
{
    return dencity;
}

void Wave::setDencity(float newDencity)
{
    if (newDencity < 0.1) {
        dencity = 0.1;
    } else if (newDencity > 1) {
        dencity = 1;
    } else {
        dencity = newDencity;
    }
}

void Wave::runEnemy()
{
    // If all the enemy have been run
    if (currentEnemyId >= groupOfEnemies.size()) {

        // Turn off timer
        timerBetweenEnemies.stop();
        QObject::disconnect(&timerBetweenEnemies, &QTimer::timeout, this, &Wave::runEnemy);

        // Quit
        return;
    }

    scene->addItem(groupOfEnemies.at(currentEnemyId));
    groupOfEnemies.at(currentEnemyId)->run();

    currentEnemyId++;

    // Setup and run timer
    timerBetweenEnemies.setInterval(dencity * 1000);
    QObject::connect(&timerBetweenEnemies, &QTimer::timeout, this, &Wave::runEnemy);
    timerBetweenEnemies.start();
}

void Wave::runEnemies(const QSharedPointer<Location> location)
{
    // Read all info about enemies and fill the list / connect enemyOutOfBattle with local slot
    setupEnemiesFromXml(QString(":/Data/Data/Locations/" + location->getName() + "/Location.xml"), location);

    // Run first enemy
    runEnemy();
}

void Wave::setupEnemiesFromXml(QString fileName,
                               const QSharedPointer<Location> location)
{
    qDebug() << "Wave::setupEnemiesFromXml" << id;
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the XML file.";
        return;
    }

    QXmlStreamReader xmlReader(&file);

    bool waveFound = false;
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {

        xmlReader.readNext();

        if (xmlReader.name() == "section") {

            if (waveFound) {
                break;
            }

            if (xmlReader.attributes().value("name").toString() == "wave" &&
                xmlReader.attributes().value("id").toInt() == id) {
                waveFound = true;
            } else {
                waveFound = false;
            }
        } else if (waveFound && xmlReader.name() == "attenemy") {

            int id = xmlReader.attributes().value("id").toInt();
            QString type = xmlReader.attributes().value("type").toString();
            int route = xmlReader.attributes().value("route").toInt();

            // Create an enemy
            auto newEnemy = new Enemy(id, type, location->getEnemyRoutes().at(route));
            groupOfEnemies.append(newEnemy);
            // Be redy to handle enemy out of battle
            QObject::connect(newEnemy, &Enemy::outOfBattle, this, &Wave::processEnemyOut);

            xmlReader.skipCurrentElement();
        }
    }

    if (xmlReader.hasError()) {
        qDebug() << "XML parsing error:" << xmlReader.errorString();
    }

    file.close();
}

void Wave::processEnemyOut(Enemy * enemy)
{
    int enemyIndex = groupOfEnemies.indexOf(enemy);
    qDebug() << "Wave::processEnemyOut: " << enemyIndex;
    if (groupOfEnemies.at(enemyIndex)) {
        scene->removeItem(groupOfEnemies.at(enemyIndex));
    }

    // Check if there any enemy is still running on the battlefield
    if (++enemiesOutOfBattleNum == groupOfEnemies.size()) {

        // clean enemies memory
        for (auto enemyItem : groupOfEnemies) {
            delete enemyItem;
        }

        groupOfEnemies.clear(); // clean enemies list;
        emit enemiesEnded();
        return;
    }

    QObject::connect(&timerBetweenEnemies, &QTimer::timeout, this, &Wave::runEnemy);
    timerBetweenEnemies.start();

}

const QList<Enemy*> &Wave::getGroupOfEnemies() const
{
    return groupOfEnemies;
}

int Wave::getId() const
{
    return id;
}

void Wave::setId(int newId)
{
    id = newId;
}
