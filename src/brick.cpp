#include "game/brick.h"

Brick::Brick(int x, int y)
    : QGraphicsPixmapItem(QPixmap::fromImage(QImage(":/data/images/Brick.png")))
{
    this->m_Type = SO_BRICK;
    this->setPos(x, y);
}

QRectF Brick::boundingRect() const
{
    return QRectF(0.0, 0.0, 32.0, 32.0);
}
