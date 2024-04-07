#ifndef LEVEL_H
#define LEVEL_H

#include <QString>
#include <QGraphicsScene>

#include "Location.h"
#include "Preferences.h"

class Level
{
public:
    Level(const Preferences * preferences,
          QGraphicsScene * scene,
          int id);
    ~Level();

    int getId() const;
    void setId(int newId);

    Location *getLocation() const;
    void setLocation(Location *newLocation);

    const QString &getStartMessage() const;
    void setStartMessage(const QString &newStartMessage);

    const QString &getVictoryMessage() const;
    void setVictoryMessage(const QString &newVictoryMessage);

private:
    void loadXmlParameters();

private:
    static const QString LEVEL_DIR_PREFIX;
    static const QString LEVEL_FILE_NAME;

    const Preferences * preferences;
    QGraphicsScene * scene;

    int id;
    Location * location;
    QString startMessage;
    QString victoryMessage;
};

#endif // LEVEL_H
