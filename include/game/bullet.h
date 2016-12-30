#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QImage>

#include "game/common.h"
#include "game/gameobject.h"

/**
 * @brief The Bullet dynamic object class
 */
class Bullet : public QGraphicsPixmapItem,
        public GameObject
{

public:
    Bullet(int x, int y, Direction_t way);

    QRectF boundingRect() const;

    void IncMove(void);

private:
    Direction_t m_ActiveDirection;
    int m_Speed;
};

#endif // BULLET_H
