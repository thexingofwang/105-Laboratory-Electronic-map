#include "scene.h"

Scene::Scene(QObject *parent) :QGraphicsScene(parent)
{
    item=new Item;
    addItem( item);
}
void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {


    emit cursorChanged(event->pos(), event->scenePos(), event->screenPos());

}
