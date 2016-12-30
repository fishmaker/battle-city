#include "game/armor.h"

Armor::Armor(int x, int y)
    : QGraphicsPixmapItem(QPixmap::fromImage(QImage(":/data/images/Armor.png")))
{
    this->m_Type = SO_ARMOR;
    this->setPos(x, y);
}

QRectF Armor::boundingRect() const
{
    return QRectF(0.0, 0.0, 32.0, 32.0);
}
