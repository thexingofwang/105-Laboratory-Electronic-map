#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "item.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

signals:
    void cursorChanged(const QPointF &pos, const QPointF &scenePos, const QPointF &screenPos);


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);   //鼠标按下事件

private:
    Item * item;

};

#endif // SCENE_H
