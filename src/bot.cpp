#include "game/bot.h"

Bot::Bot(int x, int y)
{
    this->setPixmap(QPixmap::fromImage(QImage(":/data/images/Enemy1.png")));
    this->m_Type = SO_ENEMY;
    this->m_IWantShoot = false;
    this->setPos(x, y);
    this->m_Alive = true;
    this->m_Speed = 3;

    QObject::connect(&m_ShootTimer, SIGNAL(timeout()), this, SLOT(OnTimer0_Timeout()));
    QObject::connect(&m_MovingTimer, SIGNAL(timeout()), this, SLOT(OnTimer1_Timeout()));
    QObject::connect(&m_ChangeDirection, SIGNAL(timeout()), this, SLOT(OnTimer2_Timeout()));

    m_ShootTimer.start(qrand() % 5000 + 200);
    m_MovingTimer.start(qrand() % 100 + 50); // make move
    m_ChangeDirection.start(qrand() % 1000 + 100); // change direction
}

bool Bot::WantShoot(void) const
{
    return this->m_IWantShoot;
}

void Bot::DisableShoot(void)
{
    this->m_IWantShoot = false;
}

bool Bot::GetAlive(void) const
{
    return this->m_Alive;
}

void Bot::OnTimer0_Timeout(void)
{
    this->m_IWantShoot = true;
    this->m_ShootTimer.stop();
    this->m_ShootTimer.start(qrand() % 5000);
}

void Bot::SetAlive(bool a_Alive)
{
    this->m_Alive = a_Alive;
    if (!a_Alive){
        this->m_ShootTimer.stop();
        this->m_MovingTimer.stop();
    }
}

void Bot::RandomMoving(void)
{
    qint32 a_Rand = qrand() % 5;
    this->m_ActiveDirection = (Direction_t)a_Rand;
    this->m_ForBulletDirection = this->m_ActiveDirection;
}

void Bot::GoBack(void)
{
    this->setPos(m_PrevPosition);
}

void Bot::OnTimer1_Timeout(void)
{
    m_PrevPosition = this->pos();
    if (this->m_ActiveDirection == DN_NONE) {
        // do nothing
    }

    if (this->m_ActiveDirection == DN_LEFT) {
        this->setPos(m_PrevPosition.x() - m_Speed, m_PrevPosition.y());
        this->setTransformOriginPoint(12, 12);
        this->setRotation(270);
    }

    if (this->m_ActiveDirection == DN_RIGHT) {
        this->setPos(m_PrevPosition.x() + m_Speed, m_PrevPosition.y());
        this->setTransformOriginPoint(12, 12);
        this->setRotation(90);
    }

    if (this->m_ActiveDirection == DN_DOWN) {
        this->setPos(m_PrevPosition.x(), m_PrevPosition.y() + m_Speed);
        this->setTransformOriginPoint(12, 12);
        this->setRotation(180);
    }

    if (this->m_ActiveDirection == DN_UP) {
        this->setPos(m_PrevPosition.x(), m_PrevPosition.y() - m_Speed);
        this->setTransformOriginPoint(12, 12);
        this->setRotation(0);
    }
}

void Bot::OnTimer2_Timeout(void)
{
    RandomMoving();
}
