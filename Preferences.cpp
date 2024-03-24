#include "Preferences.h"

#include <QtXml/QDomDocument>
#include <QDebug>
#include <QFile>

const QString Preferences::PREFERENCES_FILE = "./Data/Preferences/Preferences.xml";
const QString Preferences::SETTINGS_FILE = ":/Data/Data/Menu/SettingsMenu.xml";

Preferences::Preferences()
    : fullscreen(false),
      fullscreenString("Off"),
      resolutionString("800x600"),
      language("English")
{
    // Set default values for resolution
    setResolution(resolutionString);

    // Read Preferences.xml
    loadXmlParameters();
}

void Preferences::loadXmlParameters()
{
    QDomDocument MenuProcessorXml;

    QFile xmlFile(PREFERENCES_FILE);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error while loading file: " << PREFERENCES_FILE;
    }
    MenuProcessorXml.setContent(&xmlFile);
    xmlFile.close();

    QDomElement root = MenuProcessorXml.documentElement();
    QDomElement node = root.firstChild().toElement();

    // Language should be read first
    while(!node.isNull())
    {
        if (QString(node.tagName()) == "attstr") {
            if (QString(node.attribute("name")) == "language") {
                setLanguage(findOptionByNameAndIndex("language", QString(node.attribute("val")).toInt()));
            }
        }
        node = node.nextSibling().toElement();
    }

    // Read other parameters
    node = root.firstChild().toElement();
    while(!node.isNull())
    {
        if (QString(node.tagName()) == "attstr") {
            QString name = QString(node.attribute("name"));
            if (name == "fullscreen-mode") {
                setFullscreen(findOptionByNameAndIndex("fullscreen-mode", QString(node.attribute("val")).toInt()));
            } else if (name == "resolution") {
                setResolution(findOptionByNameAndIndex("resolution", QString(node.attribute("val")).toInt()));
            }
        }
        node = node.nextSibling().toElement();
    }
}

const QString &Preferences::getResolutionString() const
{
    return resolutionString;
}

const QString Preferences::findOptionByNameAndIndex(const QString &optionName,
                                                    const int &optionIndex) const
{
    QDomDocument doc;
    QFile file(SETTINGS_FILE);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open XML file for reading: " << SETTINGS_FILE;
        return QString("");
    }

    if (!doc.setContent(&file)) {
        qDebug() << "Failed to parse XML content: " << SETTINGS_FILE;
        file.close();
        return QString("");
    }

    QDomElement root = doc.documentElement();
    QDomNodeList sectionList = root.elementsByTagName("section");

    for (int i = 0, sectionListNum = sectionList.size(); i != sectionListNum; ++i) {

        QDomElement section = sectionList.at(i).toElement();
        QString name = section.attribute("name");
        QString type = section.attribute("type");

        if (type == "scroll-option" && name == optionName) {

            QDomNodeList soAttributes = section.childNodes();
            for (size_t j = 0, soAttributesNum = soAttributes.size(); j != soAttributesNum; j++) {
                auto soAttribute = soAttributes.at(j).toElement();

                if (QString(soAttribute.tagName()) == "section" &&
                        QString(soAttribute.attribute("type")) == "options") {

                    QDomNodeList textAttributes = soAttribute.childNodes().at(optionIndex).childNodes();
                    for (size_t k = 0, textAttributesNum = textAttributes.size(); k != textAttributesNum; k++) {
                        auto textAttribute = textAttributes.at(k).toElement();

                        if(QString(textAttribute.tagName()) == "attstr" &&
                           QString(textAttribute.attribute("name")).contains("text") &&
                                (QString(textAttribute.attribute("language")).contains(language) ||
                                 QString(textAttribute.attribute("language")).contains("Any"))) {
                            file.close();
                            return QString(textAttribute.attributes().namedItem("val").nodeValue());
                        }
                    }
                }
            }
        }
    }

    file.close();
    return QString("");
}

const QString &Preferences::getFullscreenString() const
{
    return fullscreenString;
}

void Preferences::setResolution(const QString &newResolution)
{
    resolutionString = newResolution;

    QStringList sizeParts = resolutionString.split('x');
    if (sizeParts.size() == 2) {
        bool ok;
        resolution.width = sizeParts[0].toInt(&ok);
        resolution.height = sizeParts[1].toInt(&ok);
        if (!ok) {
            qDebug() << "Conversion failed for resolution string: " << resolutionString;
        }
    } else {
        qDebug() << "Invalid resolution format!";
    }
}

void Preferences::saveNewOptions(const QList<ScrollOption*> & listOfOptions)
{
    QDomDocument doc;
    QFile file(PREFERENCES_FILE);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open XML file for reading: " << PREFERENCES_FILE;
        return;
    }

    if (!doc.setContent(&file)) {
        qDebug() << "Failed to parse XML content: " << PREFERENCES_FILE;
        file.close();
        return;
    }

    QDomElement root = doc.documentElement();

    for (auto option : listOfOptions) {
        QDomNodeList attstrList = root.elementsByTagName("attstr");

        for (int i = 0; i < attstrList.size(); ++i) {
            QDomElement attstr = attstrList.at(i).toElement();
            QString name = attstr.attribute("name");

            if (name == option->getTitle()) {
                attstr.setAttribute("val", option->getCurrentOptionNumber());
            }
        }
    }

    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "Failed to open XML file for writing: " << PREFERENCES_FILE;
        return;
    }

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();

    // Update parameters
    loadXmlParameters();
}

const Resolution &Preferences::getResolution() const
{
    return resolution;
}

bool Preferences::getFullscreen() const
{
    return fullscreen;
}

void Preferences::setFullscreen(bool newFullscreen)
{
    fullscreen = newFullscreen;

    if (language == "English") {
        fullscreenString = fullscreen ? "On" : "Off";
    } else if (language == "Українська") {
        fullscreenString = fullscreen ? "Вімк." : "Вимк.";
    } else if (language == "Русский") {
        fullscreenString = fullscreen ? "Вкл." : "Выкл.";
    }
}

void Preferences::setFullscreen(const QString newFullscreen)
{
    fullscreenString = newFullscreen;

    if (language == "English") {
        fullscreen = (newFullscreen == "On");
    } else if (language == "Українська") {
        fullscreen = (newFullscreen == "Вімк.");
    } else if (language == "Русский") {
        fullscreen = (newFullscreen == "Вкл.");
    }
}

const QString &Preferences::getLanguage() const
{
    return language;
}

void Preferences::setLanguage(const QString &newLanguage)
{
    language = newLanguage;
}
