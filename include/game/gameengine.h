#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <list>
#include <memory>
#include <utility>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QObject>
#include <QKeyEvent>
#include <QtGlobal>
#include <QList>
#include <QLabel>
#include <QPushButton>
#include <QMap>
#include <QSound>
#include <QMessageBox>

#include "game/tank.h"
#include "game/bullet.h"
#include "game/brick.h"
#include "game/common.h"
#include "game/armor.h"
#include "game/bot.h"

class GameEngine : public QWidget
{
    Q_OBJECT

    static const int MAX_BOTS = 5; // max alive bots

public:
    GameEngine(QWidget *parent = 0);
    ~GameEngine();

    void Prepare(void);
    void AddLabels(void);
    void MovePlayer(Tank &a_Tank);
    void SpawnBots(int x, int y);
    void HandleBots(int a_BotIndex);
    void UpdateFrag(void);
    void UpdateScore(void);
    void UpdateLives(void);
    void UpdateTime(void);

    std::pair<int, int> GetFreePosition(void);
    bool IsInside(const std::shared_ptr<Bullet> &a_Bullet);
    bool IsInside(const Tank &a_Tank);
    bool IsInside(const Bot &a_Tank);

    void keyPressEvent(QKeyEvent *a_Event);
    void keyReleaseEvent(QKeyEvent *a_Event);

private slots:
    void OnTimer0_Timeout(void);
    void OnKey0_Pressed(void);
    void OnBrickSpawn_Timeout(void);
    void OnTimeShower_Timeout(void);

private:
    QGraphicsScene m_Scene;
    QGraphicsView m_View;

    QTimer m_Timer;
    QTimer m_BrickRandomSpawner;
    QTimer m_TimeShower;

    Tank m_Tank;
    qint32 m_Lives;
    qint32 m_Frags;
    qint32 m_Score;
    int m_AliveBots;

    QList<std::shared_ptr<Bullet>> m_Bullets;
    QList<std::shared_ptr<Armor>> m_Armors;
    QList<std::shared_ptr<Brick>> m_Bricks;
    QList<std::shared_ptr<Bot>> m_Bots;

    QMap<int, bool> m_Keys;

    QLabel m_Labels[9];
    QPushButton m_PushButton;

    QSound m_Shot, m_Explosion;
};

#endif // GAMEENGINE_H
