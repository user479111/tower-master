#ifndef MENUPROCESSOR_H
#define MENUPROCESSOR_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QSharedPointer>
#include <QObject>
#include <QPointF>
#include <QList>

#include "Menu.h"
#include "BaseMenu.h"
#include "BattleMenu.h"

class MenuProcessor : public QObject
{
    Q_OBJECT
public:
    MenuProcessor(QSharedPointer<QGraphicsScene> scene);
    ~MenuProcessor();

    enum Choice { StayInMenu, StartGame, Quit };
    Choice getChoice() const;

    const QString &getLocationChoice() const;

private:
    void loadXmlParameters(QString inFileName);

private slots:
    void processItemClick();

signals:
    void keyChoiseMade();

private:
    QSharedPointer<QGraphicsScene> scene;
    QSharedPointer<Menu> currentMenu;
    QSharedPointer<BaseMenu> baseMenu;
    //CompanyMenu * subMenuCompany;
    QSharedPointer<BattleMenu> battleMenu;
    //SettingsMenu * subMenuSettings;
    //CreditsMenu * subMenuCreadits;
    Choice choice;
    QString locationChoice;

};

#endif // MENUPROCESSOR_H
