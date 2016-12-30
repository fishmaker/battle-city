#ifndef ARMOR_H
#define ARMOR_H

#include <QGraphicsPixmapItem>
#include <QImage>
#include <QDebug>

#include "game/gameobject.h"

/**
 * @brief The Armor tile block
 */
class Armor : public QGraphicsPixmapItem,
        public GameObject
{
public:
    Armor(int x, int y);

    QRectF boundingRect() const;
};

#endif // ARMOR_H
