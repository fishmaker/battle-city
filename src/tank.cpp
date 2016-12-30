#include "game/tank.h"

Tank::Tank()
    : QGraphicsPixmapItem(QPixmap::fromImage(QImage(":/data/images/Tank1.png"))),
      m_ActiveDirection(DN_NONE),
      m_ForBulletDirection(DN_UP)
{
    this->m_Type = SO_PLAYER;
    this->m_Speed = 2;
}


QRectF Tank::boundingRect() const
{
    return QRectF(0.0, 0.0,
                  24.0, 24.0);
}
