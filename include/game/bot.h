#ifndef BOT_H
#define BOT_H

#include <QTimer>
#include <QObject>

#include "game/tank.h"

/**
 * @brief The Bot class (AI enemies)
 */
class Bot : public QObject,
        public Tank
{
    Q_OBJECT
public:
    Bot(int x, int y);
    virtual ~Bot(void){}


    bool WantShoot(void) const;
    void DisableShoot(void);
    bool GetAlive(void) const;
    void SetAlive(bool a_Alive);
    void GoBack(void);
    void RandomMoving(void);

public slots:
    void OnTimer0_Timeout(void);
    void OnTimer1_Timeout(void);
    void OnTimer2_Timeout(void);

private:
    QTimer m_ShootTimer;
    QTimer m_MovingTimer;
    QTimer m_ChangeDirection;
    bool m_IWantShoot;
    bool m_Alive;
    QPointF m_PrevPosition;
};

#endif // BOT_H
