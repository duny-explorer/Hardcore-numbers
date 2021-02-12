#ifndef PRINTSCENE_H
#define PRINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

class printScene : public QGraphicsScene
{

    Q_OBJECT

public:
    explicit printScene(QObject *parent = 0);
    ~printScene();

private:
    QPointF     previousPoint;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

};

#endif // PRINTSCENE_H
