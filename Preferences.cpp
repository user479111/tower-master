#include "Preferences.h"

Preferences::Preferences()
{
    // Read Settings.xml

    // hardcode language for now
    language = "Ukrainian";
}

const QString &Preferences::getLanguage() const
{
    return language;
}

void Preferences::setLanguage(const QString &newLanguage)
{
    language = newLanguage;
}
