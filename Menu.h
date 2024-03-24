#ifndef MENU_H
#define MENU_H

#include <QFile>
#include <QList>
#include <QPointF>
#include <QObject>
#include <QGraphicsItem>
#include <QtXml/QDomDocument>

#include "ScrollOption.h"
#include "Preferences.h"
#include "MenuItem.h"

class Menu : public QObject
{
    Q_OBJECT
public:
    Menu(const Preferences * preferences,
         const QString & xmlFileName = "",
         QObject * parent = nullptr);
    ~Menu();

    virtual void show(QGraphicsScene * scene) = 0;
    virtual void hide(QGraphicsScene * scene) = 0;

    const QString transition();

    const QString &getBackgroundImage() const;
    void setBackgroundImage(const QString &newBackgroundImage);

    const QString &getBoardImage() const;
    void setBoardImage(const QString &newBoardImage);

    void addNewItem(MenuItem * newItem);
    void addNewOption(ScrollOption * option);

    QGraphicsPixmapItem * getBoard() const;

    const QList<MenuItem*> &getListOfItems() const;

    const QList<ScrollOption*> &getListOfOptions() const;

    void resetItemsLanguage();

signals:
    void itemClicked();


private slots:
    void processItemClick();

protected:
    void loadXmlParameters();

    void readMenuItemXml(MenuItem * newItem,
                         const QDomNodeList &menuItemAttributes,
                         const QString &name);

    void readScrollOptionXml(ScrollOption * newOption,
                             const QDomNodeList &scrollOptionAttributes,
                             const QString &name);

private:
    void clearItemList();
    void clearOptionList();

protected:
    static const QString MENU_DIRECTORY;

private:
    const Preferences * preferences;
    QList<MenuItem*> listOfItems;
    QList<ScrollOption*> listOfOptions;
    QString backgroundImage;
    QGraphicsPixmapItem * board;
    QString boardImage;
    QString xmlFileName;
};

#endif // MENU_H
