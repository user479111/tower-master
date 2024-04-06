#ifndef LOCATIONITEM_H
#define LOCATIONITEM_H

#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QString>
#include <QPointF>

#define HIGHLIGHT_OFFSET 5

class LocationItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    LocationItem(QString name,
                 int width,
                 int fontSize,
                 QPointF pos,
                 QGraphicsItem * parent = 0);
    ~LocationItem();

    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    const QString &getDirectoryName() const;
    void setDirectoryName(const QString &newDirectoryName);

    bool isChosen() const;
    void setChosen(bool newChosen);

    QGraphicsRectItem * getBackgroundRect() const;
    void setBackgroundRect(QGraphicsRectItem * newBackgroundRect);

    const QString &getLocationImage() const;
    void setLocationImage(const QString &newLocationImage);

    const QString &getLocationFullName() const;
    void setLocationFullName(const QString &newLocationFullName);

    int getWavesNum() const;
    void setWavesNum(int newWavesNum);

    bool getActive() const;
    void setActive(bool newActive);

    const QString &getDescription() const;
    void setDescription(const QString &newDescription);

private:
    void loadXmlParameters(QString inFileName);

signals:
    void clicked(LocationItem * choise);

private:
    QGraphicsRectItem * backgroundRect;
    QString directoryName;
    QString locationFullName;
    QString locationImage;
    QString description;
    int locationWidth;
    int locationHeight;
    int wavesNum;
    bool chosen;
    bool active;
};

#endif // LOCATIONITEM_H
