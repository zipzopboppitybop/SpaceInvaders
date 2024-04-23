#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "raylib.h"

struct WindowConfig { int W, H, FR, FS; };
struct PlayerConfig { int SW, SH, CR; float S; };
struct EnemyConfig { int SW, SH, CR, SP; float SMIN, SMAX; };
struct BulletConfig { int SW, SH, CR; float S; };

class Game
{
    EntityManager m_entities;       // vector of entities to maintain
    WindowConfig m_windowConfig;
    PlayerConfig m_playerConfig;
    EnemyConfig m_enemyConfig;
    BulletConfig m_bulletConfig;
    int m_score = 0;
    int m_currentFrame = 0;
    int m_lastEnemySpawnTime = 0;
    bool m_paused = false;
    bool m_running = true;

    std::shared_ptr<Entity> m_player;

    // Main Game Systems
    void init(const std::string& config); // Initialize Game window with config file 
    void setPaused();           // pause the game
    void sRender();         // System: Render / Drawing

    // Player and Enemy Systems
    void sMovement();       // System: Entity position / movement update
    void sUserInput();      // System: User Input
    void sLifeSpan();       // System: LifeSpan
    void sEnemySpawner();   // System: Spawns Enemies
    void sCollision();      // System: Collision
    void spawnPlayer();     
    void spawnEnemy(const Vec2& pos); 
    void spawnSpecialEnemies();
    void spawnBullet(std::shared_ptr<Entity> entity);

public:

    Game(const std::string& config); // constructor, takes in game config

    void run();
};