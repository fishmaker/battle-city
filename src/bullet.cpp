#include "game/bullet.h"

Bullet::Bullet(int x, int y, Direction_t way)
    : QGraphicsPixmapItem(QPixmap::fromImage(QImage(":/data/images/Bullet1.png"))),
      m_Speed(5)
{
    this->m_Type = SO_BULLET;
    this->m_ActiveDirection = way;

    switch (way) {
    case DN_NONE:
        break; // TODO: Assert!
    case DN_DOWN:
        this->setPos(x, y + 32);
        this->setTransformOriginPoint(2, 2);
        this->setRotation(180);
        break;
    case DN_LEFT:
        this->setPos(x - 32, y);
        this->setTransformOriginPoint(2, 2);
        this->setRotation(270);
        break;
    case DN_RIGHT:
        this->setPos(x + 32, y);
        this->setTransformOriginPoint(2, 2);
        this->setRotation(90);
        break;
    case DN_UP:
        this->setPos(x, y - 32);
        this->setTransformOriginPoint(2, 2);
        this->setRotation(0);
        break;
    }
}


QRectF Bullet::boundingRect() const
{
    return QRectF(0.0, 0.0, 4.0, 4.0);
}

void Bullet::IncMove(void)
{
    QPointF a_Current = this->pos();
    switch (this->m_ActiveDirection) {
    case DN_DOWN:
        this->setPos(a_Current.x(), a_Current.y() + this->m_Speed);
        break;
    case DN_LEFT:
        this->setPos(a_Current.x() - this->m_Speed, a_Current.y());
        break;
    case DN_RIGHT:
        this->setPos(a_Current.x() + this->m_Speed, a_Current.y());
        break;
    case DN_UP:
        this->setPos(a_Current.x(), a_Current.y() - this->m_Speed);
        break;
    case DN_NONE:
        break; // assert
    }
}
