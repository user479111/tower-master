QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BaseMenu.cpp \
    BattleMenu.cpp \
    Battlefield.cpp \
    Bullet.cpp \
    Cursor.cpp \
    Enemy.cpp \
    GameInterface.cpp \
    GameProcessor.cpp \
    Location.cpp \
    LocationItem.cpp \
    Menu.cpp \
    MenuItem.cpp \
    MenuProcessor.cpp \
    Minimap.cpp \
    Preferences.cpp \
    ScrollOption.cpp \
    SettingsMenu.cpp \
    Tower.cpp \
    TowerMaster.cpp \
    Wave.cpp \
    main.cpp

HEADERS += \
    BaseMenu.h \
    BattleMenu.h \
    Battlefield.h \
    Bullet.h \
    Cursor.h \
    Enemy.h \
    GameInterface.h \
    GameProcessor.h \
    Location.h \
    LocationItem.h \
    Menu.h \
    MenuItem.h \
    MenuProcessor.h \
    Minimap.h \
    Preferences.h \
    ScrollOption.h \
    SettingsMenu.h \
    Tower.h \
    TowerMaster.h \
    Wave.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

DISTFILES += \
    Data/Enemies/Ghost/Enemy.xml \
    Data/Locations/Borderlands/Location.xml \
    Data/Locations/Hell/Location.xml \
    Data/Menu/BaseMenu.xml \
    Data/Menu/MenuProcessor.xml \
    Data/Towers/Shooting/Tower.xml \
    README.md
