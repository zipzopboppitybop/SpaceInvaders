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

        if (temp == "Bullet")
        {
            fin >> m_bulletConfig.SW >> m_bulletConfig.SH >>m_bulletConfig.CR >> m_bulletConfig.S;
        }

    }

    if (m_windowConfig.FS == 1)
    {
       ToggleFullscreen(); 
    }

    // set up default window parameters
    InitWindow(m_windowConfig.W, m_windowConfig.H, "Invaders");
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
            sEnemySpawner();
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

    m_running = false;
    CloseWindow();
}

void Game::setPaused()
{
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
    float my = GetScreenHeight() - m_playerConfig.SH;

    entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(3.0f, 3.0f), 0.0f);

    //The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickenss 4
    entity->cShape = std::make_shared<CShape>(m_playerConfig.SW, m_playerConfig.SH, WHITE);

    // input component to control player
    entity->cInput = std::make_shared<CInput>();

    entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

    //make this entity the player
    m_player = entity;
}

// spawn an enemy at random position
void Game::spawnEnemy(const Vec2& pos)
{
    //TODO: make sure the enemy is spawned properly with the m_enemyConfig variables
    // the enemy must be spawn completely within the bounds of the window

    auto entity = m_entities.addEntity("enemy");

    entity->cTransform = std::make_shared<CTransform>(Vec2(pos.x, pos.y), Vec2(0.0f, 0.0f), 0.0f);
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
void Game::spawnBullet(std::shared_ptr<Entity> entity)
{
    auto bullet = m_entities.addEntity("bullet");
    auto entity_stuff = Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y);
    bullet->cTransform = std::make_shared<CTransform>(Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y - entity->cShape->rectangle.height), Vec2(0, m_bulletConfig.S), 0.0f);
    //bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
    bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SW, m_bulletConfig.SH, WHITE);
}

void Game::sMovement()
{
    //TODO: implement all entity movement in this function

    // player movement
    m_player->cTransform->velocity = { 0,0 };

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

        if (m_player->cTransform->pos.x < 0)
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


}

void Game::sEnemySpawner()
{
    // Spawn enemies in a grid
    int initial_x = GetScreenWidth() / 6;
    int initial_y = GetScreenHeight() / 5;

    for (int i = 0; i < 11; i++)
    {
        spawnEnemy(Vec2(initial_x + m_enemyConfig.SW * (i % 11) + 20.0f * (i % 11), initial_y));
    }
    
}


// Renders the game
void Game::sRender()
{
    // Clear the window every frame so that the same entity doesn't get drawn over and over
    BeginDrawing();
    ClearBackground(BLACK);

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

    if (IsKeyPressed(KEY_P)) 
    {
        setPaused();
    }

    if (!m_paused)
    {
        // Keys being pressed
        // Movement keys
        if (IsKeyDown(KEY_A)) 
        {
            m_player->cInput->left = true;
           
        }

        if (IsKeyDown(KEY_D))
        {
            m_player->cInput->right = true;

        }

        // Attack keys
        if (IsKeyPressed(KEY_SPACE))
        {
            m_player->cInput->shoot = true;
            spawnBullet(m_player);
        }


        // Keys being released
        // Movement keys
        if (IsKeyReleased(KEY_A))
        {
            m_player->cInput->left = false;

        }

        if (IsKeyReleased(KEY_D))
        {
            m_player->cInput->right = false;

        }
        // Attack keys
        if (IsKeyReleased(KEY_SPACE))
        {
            m_player->cInput->shoot = false;
        }
    }
}