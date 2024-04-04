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
    Cursor(QGraphicsScene * scene);

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

    bool getScrollingAllowed() const;
    void setScrollingAllowed(bool newScrollingAllowed);

private slots:
    void checkScrollArea();

signals:
    void areaScrolled();
    void mousePressed();
    void cursorMoved();

private:
    static const int MARGIN_CLOSE;
    static const int FAST_SCROLL;

    static const int MARGIN_FAR;
    static const int SLOW_SCROLL;
    QGraphicsScene * scene;

    QTimer scrollTimer;

    QRectF scrollAreaRect;
    QString towerType;

    bool buildMode;
    bool buildIsPossible;
    bool scrollingAllowed;
};

#endif // CURSOR_H
