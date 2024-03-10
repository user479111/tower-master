#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>

class Preferences
{
public:
    Preferences();

    const QString &getLanguage() const;
    void setLanguage(const QString &newLanguage);

private:
    QString language;
};

#endif // PREFERENCES_H
