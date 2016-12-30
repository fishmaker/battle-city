#ifndef TANK_H
#define TANK_H

#include <QGraphicsPixmapItem>
#include <QImage>
#include <QTransform>

#include "game/gameobject.h"
#include "game/common.h"

class Tank : public QGraphicsPixmapItem,
        public GameObject
{
public:
    Tank();
    QRectF boundingRect() const;

    Direction_t GetActiveDirection(void) const { return this->m_ActiveDirection; }
    void SetActiveDirection(const Direction_t a_Type) { this->m_ActiveDirection = a_Type; }

    Direction_t GetBulletDirection(void) const { return this->m_ForBulletDirection; }
    void SetBulletDirection(const Direction_t a_Type) { this->m_ForBulletDirection = a_Type; }

    qint32 GetSpeed() const { return this->m_Speed; }

protected:
    Direction_t m_ActiveDirection;
    Direction_t m_ForBulletDirection;
    qint32 m_Speed;
};

#endif // TANK_H
