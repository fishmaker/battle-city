#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "game/common.h"

class GameObject
{
public:
    GameObject();

    virtual ObjectType_t GetType(void) const { return this->m_Type; }

protected:
    ObjectType_t m_Type;
};

#endif // GAMEOBJECT_H
