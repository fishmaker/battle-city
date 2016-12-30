#ifndef BRICK_H
#define BRICK_H

#include <QGraphicsPixmapItem>
#include <QImage>
#include <QDebug>

#include "game/gameobject.h"

/**
 * @brief The Brick tile
 */
class Brick : public QGraphicsPixmapItem,
        public GameObject
{
public:
    Brick(int x, int y);

    QRectF boundingRect() const;
};

#endif // BRICK_H
