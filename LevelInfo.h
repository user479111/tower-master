#ifndef LEVELINFO_H
#define LEVELINFO_H

#include <QString>

#include "Preferences.h"

class LevelInfo
{
public:
    LevelInfo(const Preferences * preferences,
              int id);

    int getId() const;
    void setId(int newId);

    const QString &getLocationDirName() const;
    void setLocationDirName(const QString &newLocationDirName);
    const QString &getLocationName() const;
    void setLocationName(const QString &newLocationName);
    const QString &getLocationImage() const;
    void setLocationImage(const QString &newLocationImage);
    const QString &getTitle() const;
    void setTitle(const QString &newTitle);
    int getWavesNum() const;
    void setWavesNum(int newWavesNum);

private:
    void loadXmlParameters();

private:
    static const QString LOCATIONS_DIRECTORY;
    static const QString LOCATION_FILE_NAME;
    static const QString LEVEL_DIR_PREFIX;
    static const QString LEVEL_FILE_NAME;

    const Preferences * preferences;

    int id;
    QString locationDirName;
    QString locationName;
    QString locationImage;
    QString title;
    int wavesNum;
};

#endif // LEVELINFO_H
