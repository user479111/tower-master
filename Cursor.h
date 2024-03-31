#ifndef CURSOR_H
#define CURSOR_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QPointF>
#include <QObject>
#include <QRectF>
#include <QTimer>

class Cursor : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Cursor(QGraphicsScene * scene, const QRectF &inScrollAreaRect);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    const QRectF &getScrollAreaRect() const;
    void setScrollAreaRect(const QRectF &newScrollAreaRect);

    bool getBuildMode() const;
    void setBuildMode(const bool newBuildMode, const QString &towerType = 0, const qreal scale = 1);

    void setPos(const QPointF &pos);

    void setScale(qreal scale);

    void setBuildImage(bool buildIsPossible);

    bool getBuildIsPossible() const;

    void processEscapePress();

private slots:
    void checkScrollArea();

signals:
    void areaScrolled();
    void mousePressed();
    void cursorMoved();

private:
    QGraphicsScene * scene;

    QTimer scrollTimer;

    QRectF scrollAreaRect;
    QString towerType;

    bool buildMode;
    bool buildIsPossible;
};

#endif // CURSOR_H
