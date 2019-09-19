#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include "spline.h"

struct coordinate
{
    float x;
    float y;
};
tk::spline s1,s2;
static  QPainterPath dense_path_cp;

class Item : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Item(QObject *parent = nullptr);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    void fit_line(QString fit_position);


private:
    QRectF m_boundingRect;
    coordinate bd2local(QString bd_position);

};

#endif // ITEM_H
