#include "game/gameengine.h"

GameEngine::GameEngine(QWidget *parent)
    : QWidget(parent),
      m_Scene(0, 0, 512, 512, this),
      m_Shot(":/data/sound/shot.wav"),
      m_Explosion(":/data/sound/explosion.wav")
{
    QObject::connect(&this->m_Timer, SIGNAL(timeout()), this, SLOT(OnTimer0_Timeout()));
    QObject::connect(&this->m_PushButton, SIGNAL(clicked()), this, SLOT(OnKey0_Pressed()));
    QObject::connect(&this->m_BrickRandomSpawner, SIGNAL(timeout()), this, SLOT(OnBrickSpawn_Timeout()));
    QObject::connect(&this->m_TimeShower, SIGNAL(timeout()), this, SLOT(OnTimeShower_Timeout()));

    this->setGeometry(0, 0, 700, 512);

    this->m_Frags = this->m_Score = 0;
    this->m_Lives = 3;
    this->m_AliveBots = 0;

    this->m_Timer.start(15);
    this->m_TimeShower.start(1000);
    this->m_BrickRandomSpawner.start(1000);

    m_Scene.setBackgroundBrush(Qt::black);
    m_Scene.addItem(&m_Tank);

    m_View.setParent(this);
    m_View.setGeometry(0, 0, 512, 512);
    m_View.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_View.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_View.setScene(&m_Scene);
    m_View.show();
}

GameEngine::~GameEngine(void)
{}

void GameEngine::OnKey0_Pressed(void)
{
    this->close();
}

void GameEngine::OnBrickSpawn_Timeout(void)
{
    std::pair<int, int> a_Pos = GetFreePosition();
    std::shared_ptr<Brick> tmp(new Brick(a_Pos.first, a_Pos.second));
    this->m_Bricks.push_back(tmp);
    this->m_Scene.addItem(tmp.get());
}

void GameEngine::OnTimeShower_Timeout(void)
{
    UpdateTime();
}

void GameEngine::OnTimer0_Timeout(void)
{
    // Move tank
    MovePlayer(m_Tank);

    // Spawn Bots
    for (auto i = m_AliveBots; i < MAX_BOTS; ++i) {
        std::pair<int, int> tmp = GetFreePosition();
        SpawnBots(tmp.first, tmp.second);
    }

    // Handle Bots
    for (auto i = 0; i < m_Bots.size(); ++i) {
        HandleBots(i);
    }

    // Handle Bullets
    // Cant use iterator here, because dynamic adding/removing items from the list
    for (auto it=0; it < m_Bullets.size(); ++it) {
        // First - check bounds
        if (!IsInside(m_Bullets.at(it))) {
            this->m_Scene.removeItem((m_Bullets.at(it)).get());
            this->m_Bullets.removeOne(m_Bullets.at(it));
            continue;
        }
        // Second - colliding items
        QList<QGraphicsItem*> m_Others;
        if (!((m_Others = (m_Bullets.at(it))->collidingItems()).empty())) {
            GameObject* tmp = dynamic_cast<GameObject*>(m_Others.first());
            ObjectType_t a_Type = tmp->GetType();
            if (a_Type == SO_BRICK) {
                m_Explosion.play();
                this->m_Scene.removeItem(m_Others.first());
                this->m_Score += 300;
            }

            if (a_Type == SO_ENEMY) {
                m_Explosion.play();
                // Find Bot
                for (auto jt = this->m_Bots.begin(); jt != m_Bots.end(); ++jt) {
                    if ((*jt).get() == m_Others.first()) {
                        (*jt)->SetAlive(false);
                        this->m_Scene.removeItem(m_Others.first());
                        this->m_Bots.removeOne(*jt);
                        break;
                    }
                }
                this->m_AliveBots--;
                this->m_Frags++;
                this->m_Score += 1000;
            }

            if (a_Type == SO_PLAYER) {
                this->m_Lives--;
                // Destroy tank
                // Spawn Tank
            }

            // Remove bullet
            this->m_Scene.removeItem((m_Bullets.at(it)).get());
            this->m_Bullets.removeOne(m_Bullets.at(it));
        } else {
            (m_Bullets.at(it))->IncMove();
        }
    }

    // Update lables
    UpdateFrag();
    UpdateScore();
    UpdateLives();
}


void GameEngine::keyPressEvent(QKeyEvent *a_Event)
{
    m_Keys[a_Event->key()] = true;
    QWidget::keyPressEvent(a_Event);
}

void GameEngine::MovePlayer(Tank &a_Tank)
{
    QPointF a_Current = a_Tank.pos();
    qint32 a_Speed = a_Tank.GetSpeed();

    if (m_Keys[Qt::Key_W]) {
        m_Tank.SetBulletDirection(DN_UP);
        m_Tank.SetActiveDirection(DN_UP);
        a_Tank.setPos(a_Current.x(), a_Current.y() - a_Speed);
        a_Tank.setTransformOriginPoint(12, 12);
        a_Tank.setRotation(0);
    }

    if (m_Keys[Qt::Key_S]) {
        m_Tank.SetBulletDirection(DN_DOWN);
        m_Tank.SetActiveDirection(DN_DOWN);
        a_Tank.setPos(a_Current.x(), a_Current.y() + a_Speed);
        a_Tank.setTransformOriginPoint(12, 12);
        a_Tank.setRotation(180);
    }

    if (m_Keys[Qt::Key_A]) {
        m_Tank.SetBulletDirection(DN_LEFT);
        m_Tank.SetActiveDirection(DN_LEFT);
        a_Tank.setPos(a_Current.x() - a_Speed, a_Current.y());
        a_Tank.setTransformOriginPoint(12, 12);
        a_Tank.setRotation(270);
    }

    if (m_Keys[Qt::Key_D]) {
        m_Tank.SetBulletDirection(DN_RIGHT);
        m_Tank.SetActiveDirection(DN_RIGHT);
        a_Tank.setPos(a_Current.x() + a_Speed, a_Current.y());
        a_Tank.setTransformOriginPoint(12, 12);
        a_Tank.setRotation(90);
    }

    if (m_Keys[Qt::Key_Space]) {
        m_Shot.play();
        std::shared_ptr<Bullet> tmp(new Bullet(this->m_Tank.pos().x() + 11, this->m_Tank.pos().y() + 11, m_Tank.GetBulletDirection()));
        this->m_Bullets.push_back(tmp);
        this->m_Scene.addItem(tmp.get());
    }

    // Position changed - stop tank
    m_Tank.SetActiveDirection(DN_NONE);


    // If bad position - roll back
    if (!m_Tank.collidingItems().empty() || !IsInside(m_Tank)) {
        m_Tank.setPos(a_Current);
    }
}

void GameEngine::keyReleaseEvent(QKeyEvent *a_Event)
{
    m_Keys[a_Event->key()] = false;
    QWidget::keyReleaseEvent(a_Event);
}

void GameEngine::Prepare(void)
{
    // Use random spawner
    for (int i=0; i<16; i++) {
        for (int j=0; j<16; j++) {
            int k = qrand();
            if (k % 4 == 0) {
                std::shared_ptr<Brick> tmp(new Brick(j*32, i*32));
                this->m_Bricks.push_back(tmp);
                this->m_Scene.addItem(tmp.get());
            } else if (k % 5 == 0) {
                std::shared_ptr<Armor> tmp(new Armor(j*32, i*32));
                this->m_Armors.push_back(tmp);
                this->m_Scene.addItem(tmp.get());
            } else if (k % 7 == 0) {
                if (this->m_AliveBots < MAX_BOTS) {
                    SpawnBots(j*32, i*32);
                }
            }
        }
    }
}

// TODO: Make common function, and call it with different argument type
bool GameEngine::IsInside(const std::shared_ptr<Bullet> &a_Bullet)
{
    bool a_Result = true;
    QPointF a_BPos = a_Bullet->pos();
    if (a_BPos.x() < 0 || a_BPos.x() > 512 || a_BPos.y() < 0 || a_BPos.y() > 512)
        a_Result = false;
    return a_Result;
}

bool GameEngine::IsInside(const Tank &a_Tank)
{
    bool a_Result = true;
    QPointF a_UpPoint = a_Tank.pos();
    QPointF a_DownPoint = QPointF(a_UpPoint.x() + 24, a_UpPoint.y() + 24);
    if (a_UpPoint.x() < 0 || a_DownPoint.x() > 512 || a_UpPoint.y() < 0 || a_DownPoint.y() > 512)
        a_Result = false;
    return a_Result;
}

bool GameEngine::IsInside(const Bot &a_Bot)
{
    bool a_Result = true;
    QPointF a_UpPoint = a_Bot.pos();
    QPointF a_DownPoint = QPointF(a_UpPoint.x() + 24, a_UpPoint.y() + 24);
    if (a_UpPoint.x() < 0 || a_DownPoint.x() > 512 || a_UpPoint.y() < 0 || a_DownPoint.y() > 512)
        a_Result = false;
    return a_Result;
}

void GameEngine::AddLabels(void)
{
    this->m_Labels[0].setParent(this);
    this->m_Labels[0].setGeometry(550, 20, 131, 51);
    this->m_Labels[0].setFont(QFont("Sans Serif", 24));
    this->m_Labels[0].setText("Level 1");
    this->m_Labels[0].show();

    this->m_Labels[1].setParent(this);
    this->m_Labels[1].setGeometry(530, 280, 151, 31);
    this->m_Labels[1].setFont(QFont("Sans Serif", 14));
    this->m_Labels[1].setAlignment(Qt::AlignCenter);
    this->m_Labels[1].setText("Frags");
    this->m_Labels[1].show();

    this->m_Labels[2].setParent(this);
    this->m_Labels[2].setGeometry(530, 350, 151, 31);
    this->m_Labels[2].setFont(QFont("Sans Serif", 14));
    this->m_Labels[2].setAlignment(Qt::AlignCenter);
    this->m_Labels[2].setText("Score");
    this->m_Labels[2].show();

    this->m_Labels[3].setParent(this);
    this->m_Labels[3].setGeometry(530, 210, 151, 31);
    this->m_Labels[3].setFont(QFont("Sans Serif", 14));
    this->m_Labels[3].setAlignment(Qt::AlignCenter);
    this->m_Labels[3].setText("Time");
    this->m_Labels[3].show();

    this->m_Labels[4].setParent(this);
    this->m_Labels[4].setGeometry(530, 240, 151, 20);
    this->m_Labels[4].setFont(QFont("Sans Serif", 9));
    this->m_Labels[4].setAlignment(Qt::AlignCenter);
    this->m_Labels[4].setText("00:00:00");
    this->m_Labels[4].show();

    this->m_Labels[5].setParent(this);
    this->m_Labels[5].setGeometry(530, 310, 151, 20);
    this->m_Labels[5].setFont(QFont("Sans Serif", 9));
    this->m_Labels[5].setAlignment(Qt::AlignCenter);
    this->m_Labels[5].setText("Frags counter");
    this->m_Labels[5].show();

    this->m_Labels[6].setParent(this);
    this->m_Labels[6].setGeometry(530, 380, 151, 20);
    this->m_Labels[6].setFont(QFont("Sans Serif", 9));
    this->m_Labels[6].setAlignment(Qt::AlignCenter);
    this->m_Labels[6].setText("Score counter");
    this->m_Labels[6].show();

    this->m_Labels[7].setParent(this);
    this->m_Labels[7].setGeometry(530, 150, 151, 31);
    this->m_Labels[7].setFont(QFont("Sans Serif", 14));
    this->m_Labels[7].setAlignment(Qt::AlignCenter);
    this->m_Labels[7].setText("Lives");
    this->m_Labels[7].show();

    this->m_Labels[8].setParent(this);
    this->m_Labels[8].setGeometry(530, 180, 151, 20);
    this->m_Labels[8].setFont(QFont("Sans Serif", 9));
    this->m_Labels[8].setAlignment(Qt::AlignCenter);
    this->m_Labels[8].setText("3");
    this->m_Labels[8].show();

    this->m_PushButton.setParent(this);
    this->m_PushButton.setGeometry(540, 450, 131, 40);
    this->m_PushButton.setText("Exit");
    this->m_PushButton.show();

}

void GameEngine::HandleBots(int a_BotIndex)
{
    // Shot
    if ((this->m_Bots.at(a_BotIndex))->WantShoot()) {
        std::shared_ptr<Bullet> tmp(new Bullet((this->m_Bots.at(a_BotIndex))->pos().x() + 15, (this->m_Bots.at(a_BotIndex))->pos().y() + 15, (this->m_Bots.at(a_BotIndex))->GetBulletDirection()));
        this->m_Bullets.push_back(tmp);
        this->m_Scene.addItem(tmp.get());
        this->m_Bots.at(a_BotIndex)->DisableShoot();
    }
    // Check bounds
    if (!this->m_Bots.at(a_BotIndex)->collidingItems().empty() || !IsInside(*(this->m_Bots.at(a_BotIndex)))) {
        this->m_Bots.at(a_BotIndex)->GoBack();
    }
}

void GameEngine::SpawnBots(int x, int y)
{
    std::shared_ptr<Bot> tmp(new Bot(x, y));
    this->m_Bots.push_back(tmp);
    this->m_Scene.addItem(tmp.get());
    this->m_AliveBots++;
}

void GameEngine::UpdateFrag(void)
{
    this->m_Labels[5].setText(QString::number(this->m_Frags));
}

void GameEngine::UpdateLives(void)
{
    if (this->m_Lives <= -1) {
        // YOU LOSE
        QMessageBox a_MsgBox;
        this->m_Lives = 0;
        this->m_Bots.clear();
        this->m_Bricks.clear();
        this->m_Bullets.clear();
        this->m_Armors.clear();
        this->m_Timer.stop();
        this->m_BrickRandomSpawner.stop();
        a_MsgBox.setText("YOU LOSE!");
        a_MsgBox.exec();
    }
    this->m_Labels[8].setText(QString::number(this->m_Lives));
}

void GameEngine::UpdateScore(void)
{
    this->m_Labels[6].setText(QString::number(this->m_Score));
}

void GameEngine::UpdateTime(void)
{
    static const std::time_t a_SinceAppStart = std::time(nullptr); // Seconds Since Epoch

    int hours, minutes, seconds;
    struct tm a_Time;
    char buffer[16];
    int a_Diff = int(std::difftime(std::time(nullptr), a_SinceAppStart));

    hours = a_Diff / 3600;
    minutes = (a_Diff - hours * 60) / 60;
    seconds = a_Diff - minutes * 60;
    a_Time.tm_hour = hours;
    a_Time.tm_min = minutes;
    a_Time.tm_sec  = seconds;
    strftime(buffer, 16, "%H:%M:%S", &a_Time);
    this->m_Labels[4].setText(QString(buffer));
}

std::pair<int, int> GameEngine::GetFreePosition(void)
{
    // by recursion
    int x = qrand() % 16; x *= 32;
    int y = qrand() % 16; y *= 32;
    if (this->m_Scene.itemAt(QPointF(x, y), this->m_View.transform()) == nullptr) {
        return std::make_pair(x, y);
    } else {
        return GetFreePosition();
    }
}
