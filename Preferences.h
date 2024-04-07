#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>

#include "ScrollOption.h"

struct Resolution {
    int width;
    int height;
};

class Preferences
{
public:
    enum GameMode {
        Company
      , Battle
    };

    Preferences();

    const QString &getLanguage() const;
    void setLanguage(const QString &newLanguage);

    bool getFullscreen() const;
    void setFullscreen(bool newFullscreen);
    void setFullscreen(const QString newFullscreen);

    const Resolution &getResolution() const;
    void setResolution(const QString &newResolution);

    void saveNewOptions(const QList<ScrollOption*> & listOfOptions);

    const QString &getFullscreenString() const;

    const QString &getResolutionString() const;

    const QString findOptionByNameAndIndex(const QString &optionName,
                                           const int &optionIndex) const;

    GameMode getGameMode() const;

    void setGameMode(GameMode newGameMode);

    // Update SaveCompany.xml
    void activateNextCompanyLevel(int id) const;

private:
    void loadXmlParameters();

private:
    static const QString PREFERENCES_FILE;
    static const QString SETTINGS_FILE;

    static const QString SAVE_COMPANY_FILE;

    bool        fullscreen;
    QString     fullscreenString;
    Resolution  resolution;
    QString     resolutionString;
    QString     language;

    GameMode    gameMode;

};

#endif // PREFERENCES_H
