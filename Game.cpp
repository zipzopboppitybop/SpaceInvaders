#include "Game.h"
#include <fstream>
#include <iostream>

Game::Game(const std::string& config)
{
    init(config);
}

void Game::init(const std::string& path)
{
    std::ifstream fin(path);
    std::string temp;
    while (fin >> temp)
    {
        if (temp == "Window")
        {
            fin >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.FR >> m_windowConfig.FS;
        }

        if (temp == "Player")
        {
            fin >> m_playerConfig.SW >> m_playerConfig.SH >> m_playerConfig.CR >> m_playerConfig.S ;
        }

        if (temp == "Enemy")
        {
            fin >> m_enemyConfig.SW >> m_enemyConfig.SH >> m_enemyConfig.CR >> m_enemyConfig.SP >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX;
        }

        //if (temp == "Bullet")
        //{
            //fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
        //}

    }

    if (m_windowConfig.FS == 1)
    {
       ToggleFullscreen(); 
    }

    // set up default window parameters
    InitWindow(m_windowConfig.W, m_windowConfig.H, "My first RAYLIB program!");
    SetTargetFPS(m_windowConfig.FR);

    spawnPlayer();
}

void Game::run()
{
    while (!WindowShouldClose())
    {
        m_entities.update();

        if (!m_paused)
        {
            //sEnemySpawner();
            sMovement();
            sCollision();
            m_currentFrame++;
        }

        sRender();
        sUserInput();

        // respawn player if they die
        if (!m_player->isActive())
        {
            spawnPlayer();
        }
    }

    CloseWindow();
}

void Game::setPaused()
{
    //TODO
    m_paused = !m_paused;
}

// respawn the player in the middle of the screen
void Game::spawnPlayer()
{
    //TODO: finish adding all properties of the player with the correct values from the config

    // We Create every entity by calling EntityManager.addEnitity(tag)
    //This returns a std::shared_pty<Entity>, so we use 'auto' to save typing
    auto entity = m_entities.addEntity("player");

    //Give this entity a transform so it spawns at middle of screen with velocity (1,1) and angle 0
    float mx = GetScreenWidth() / 2.0f;
    float my = GetScreenHeight() / 2.0f;

    entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(1.0f, 1.0f), 0.0f);

    //The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickenss 4
    entity->cShape = std::make_shared<CShape>(m_playerConfig.SW, m_playerConfig.SH, WHITE);

    // input component to control player
    entity->cInput = std::make_shared<CInput>();

    entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

    //make this entity the player
    m_player = entity;
}

// spawn an enemy at random position
void Game::spawnEnemy()
{
    //TODO: make sure the enemy is spawned properly with the m_enemyConfig variables
    // the enemy must be spawn completely within the bounds of the window

    auto entity = m_entities.addEntity("enemy");

    float ex = rand() % GetScreenWidth();
    float ey = rand() % GetScreenHeight();
    auto center = Vec2(GetScreenWidth(), GetScreenHeight());
    auto enemyPos = Vec2(ex, ey);
    float angle = -1 + (rand() % (1 + 1 + 1));
    float randomS = m_enemyConfig.SMIN + (rand() % 1 + m_enemyConfig.SMAX - m_enemyConfig.SMIN);
    float randomFR = 0 + (rand() % 255);
    float randomFG = 0 + (rand() % 255);
    float randomFB = 0 + (rand() % 255);
    auto normalize = center.normalize(enemyPos);

    entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(randomS * normalize.x, randomS * normalize.y), angle);
    entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
    entity->cShape = std::make_shared<CShape>(m_enemyConfig.SW, m_enemyConfig.SH, WHITE);

    //record when the most enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

// spawns special enemies randomly
void Game::spawnSpecialEnemies()
{
    //TODO: spawn small enemies at the location of input enemy e
   // when we create the smaller enemy, we have to read the values of the original enemy
   // - spawn a number of small enemies equal to the vertices of the original enemy
   // - set each small enemy ot thte same color as the original, half the size
   // - small enemies are worth double points of the original enemy

}


// spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
    // TODO: implement the spawning of a bullet which travels toward target
    // -bullet speed is given as a scalar speed
    // - you must set the velocity using formula in notes

    //auto bullet = m_entities.addEntity("bullet");
    //auto entity_stuff = Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y);
    //float angle = target.angle(entity_stuff);
    //auto normalize = target.normalize(entity_stuff);
    //bullet->cTransform = std::make_shared<CTransform>(Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y), Vec2(m_bulletConfig.S * normalize.x, m_bulletConfig.S * normalize.y), angle);
   // bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
    //bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
}

void Game::sMovement()
{
    //TODO: implement all entity movement in this function

    // player movement
    m_player->cTransform->velocity = { 0,0 };

    if (m_player->cInput->up)
    {
        m_player->cTransform->velocity.y += -m_playerConfig.S;

        if (m_player->cTransform->pos.y - m_player->cCollision->radius < 0)
        {
            m_player->cTransform->velocity.y = 0;
        }
    }

    if (m_player->cInput->down)
    {
        m_player->cTransform->velocity.y += m_playerConfig.S;

        if (m_player->cTransform->pos.y + m_player->cCollision->radius > GetScreenHeight())
        {
            m_player->cTransform->velocity.y = 0;

        }
    }

    if (m_player->cInput->right)
    {
        m_player->cTransform->velocity.x += m_playerConfig.S;

        if (m_player->cTransform->pos.x + m_player->cCollision->radius > GetScreenWidth())
        {
            m_player->cTransform->velocity.x = 0;

        }
    }

    if (m_player->cInput->left)
    {
        m_player->cTransform->velocity.x += -m_playerConfig.S;

        if (m_player->cTransform->pos.x - m_player->cCollision->radius < 0)
        {
            m_player->cTransform->velocity.x = 0;

        }
    }

    // move all entities in m_entities vector
    for (auto e : m_entities.getEntities())
    {
        e->cTransform->pos.x += e->cTransform->velocity.x;
        e->cTransform->pos.y += e->cTransform->velocity.y;
    }

    for (auto e : m_entities.getEntities("enemy"))
    {
        if (e->cTransform->pos.x - e->cCollision->radius < 0)
        {
            e->cTransform->velocity.x *= -1;
        }

        if (e->cTransform->pos.x + e->cCollision->radius > GetScreenWidth())
        {
            e->cTransform->velocity.x *= -1;
        }

        if (e->cTransform->pos.y - e->cCollision->radius < 0)
        {
            e->cTransform->velocity.y *= -1;
        }

        if (e->cTransform->pos.y + e->cCollision->radius > GetScreenHeight())
        {
            e->cTransform->velocity.y *= -1;
        }
    }

}

void Game::sLifeSpan()
{
    //TODO: implent all lifespan functionality
    // for all entities
    // if entity has no lifespan component, skip it
    // if entity has > 0 remaining lifespan, subtract 1
    // if it has lifespan and is alive sclae its alpha channel properly
    // if it has lifesapn and its time is up destroy it
}

void Game::sCollision()
{
    //TODO: implement all proper collisions between entities
    // use collision radius not shape radius

    for (auto b : m_entities.getEntities("bullet"))
    {
        for (auto e : m_entities.getEntities("enemy"))
        {
            auto enemy_stuff = Vec2(e->cTransform->pos.x, e->cTransform->pos.y);
            auto bullet_stuff = Vec2(b->cTransform->pos.x, b->cTransform->pos.y);
            float dist = bullet_stuff.dist(enemy_stuff);
            if (dist < b->cCollision->radius + e->cCollision->radius)
            {
                b->destroy();
                e->destroy();
            }
        }
    }

    for (auto e : m_entities.getEntities("enemy"))
    {
        auto enemy_stuff = Vec2(e->cTransform->pos.x, e->cTransform->pos.y);
        auto player_stuff = Vec2(m_player->cTransform->pos.x, m_player->cTransform->pos.y);
        float dist = player_stuff.dist(enemy_stuff);
        if (dist < e->cCollision->radius + m_player->cCollision->radius)
        {
            e->destroy();
            m_player->destroy();
        }
    }
}

void Game::sEnemySpawner()
{
    // Spawn an enemy by subtracting the last time an enemy was spawned and the current frame to equal 3 seconds
    std::cout << m_lastEnemySpawnTime << std::endl;
    if (m_currentFrame - m_lastEnemySpawnTime == m_enemyConfig.SP) spawnEnemy();
}


// Renders the game
void Game::sRender()
{
    // Clear the window every frame so that the same entity doesn't get drawn over and over
    Color darkGreen = Color{ 20, 160, 133, 255 };
    BeginDrawing();
    ClearBackground(darkGreen);

    for (auto e : m_entities.getEntities())
    {
        // set the position of the shape based on the entity's transform->pos

        //set the rotation of the shape based on the entity's transform->angle
        //e->cTransform->angle += 1.0f;
        //e->cShape->circle.setRotation(e->cTransform->angle);

        // draw entities sf::CircleShape
        e->cShape->rectangle.Draw(Vec2(e->cTransform->pos.x, e->cTransform->pos.y));
    }

    EndDrawing();
}

void Game::sUserInput()
{

}