#include "printscene.h"

printScene::printScene(QObject *parent) : QGraphicsScene(parent)
{

}

printScene::~printScene()
{

}

void printScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    addEllipse(event->scenePos().x() - 5,
               event->scenePos().y() - 5,
               10,
               10,
               QPen(Qt::NoPen),
               QBrush(Qt::white));
    previousPoint = event->scenePos();
}

void printScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    addLine(previousPoint.x(),
            previousPoint.y(),
            event->scenePos().x(),
            event->scenePos().y(),
            QPen(Qt::white,10,Qt::SolidLine,Qt::RoundCap));
    previousPoint = event->scenePos();
}
