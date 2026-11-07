#pragma once
#include "Players.h"

class Enemy : public CombatSprite
{
protected:
    bool is_pushable = true;
    
    const PlayerManager player_manager;
    
public:
    Enemy(
        const Vec2f& _position, 
        const Camera& _camera, 
        const PlayerManager& _player_manager, 
        doubly_linked_list<Projectile*>& projectile_manager, 
        Entity_Data entity_data); 
    
    virtual void defeat() override;
    
    bool get_is_pushable() const;
    
protected:
    // Returns the distance to a closest target
    HeroEntity* find_closest_hero() const;
};

class MovingEnemy;
class EnemyManager;

class EnemySpawner : public Enemy
{
protected:
    Spawner_Data spawner_data;
    float time_since_last_spawn;
    unsigned int number_of_enemies = 0;
    
    EnemyManager& enemy_manger;
    
public:
    EnemySpawner(
        const Vec2f& _position, 
        const Camera& _camera, 
        const PlayerManager& _player_manager,
        EnemyManager& _enemy_manager,
        doubly_linked_list<Projectile*>& projectile_manager, 
        Spawner_Data _spawner_data); 
    
    virtual void update(float delta_time) override;
    
protected:
    void spawn_enemy();
};

class MovingEnemy : public Enemy
{
public:
    MovingEnemy(
        const Vec2f& _position, 
        const Camera& _camera, 
        const PlayerManager& _player_manager, 
        doubly_linked_list<Projectile*>& projectile_manager, 
        Entity_Data entity_data); 
    
    void move(float delta_time, HeroEntity* target);
    virtual void update(float delta_time) override;
};
