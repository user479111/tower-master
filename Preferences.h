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

private:
    void loadXmlParameters();

private:
    static const QString PREFERENCES_FILE;
    static const QString SETTINGS_FILE;

    bool        fullscreen;
    QString     fullscreenString;
    Resolution  resolution;
    QString     resolutionString;
    QString     language;

};

#endif // PREFERENCES_H
