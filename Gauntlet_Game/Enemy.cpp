#include "Enemy.h"
#include "Enemy_Manager.h"
#include <iostream>

Enemy::Enemy(
    const Vec2f& _position, 
    const Camera& _camera, 
    const PlayerManager& _player_manager,
    doubly_linked_list<Projectile*>& projectile_manager, 
    Entity_Data entity_data) 
: CombatEntity(_position, _camera, projectile_manager, entity_data), player_manager(_player_manager){}

void Enemy::defeat() {
    alive = false;
}

bool Enemy::get_is_pushable() const {
    return is_pushable;
}

// Target hero is:
// 1. The hero with the lowest attack priority. (LOW, NORMAL, HIGH)
// 2. If multiple have the same priority (This will happen most of the time) the closest hero is chosen
Player* Enemy::find_target_hero() const {
    float target_distance = std::numeric_limits<float>::max();
    Attack_Priority target_attack_priority = LOW_PRIORITY;
    Player* target_hero = nullptr;
    
    for (unsigned int i = 0; i < player_manager.get_player_count(); i++) {
        if (player_manager.players[i] == nullptr || !player_manager.players[i]->is_alive()) continue;
        
        Attack_Priority attack_priority = player_manager.players[i]->get_enemy_attack_priority();
        
        // Ignore if lower priority then current target
        // Doing this check here means we don't need to calculate distance unnecessarily
        if (attack_priority < target_attack_priority) continue;
        
        float distance_squared = get_centre().distanceSquared(player_manager.players[i]->get_centre());
        if (distance_squared < target_distance || attack_priority > target_attack_priority) {
            target_hero = player_manager.players[i];
            target_attack_priority = attack_priority;
            target_distance = distance_squared;
        }
        
    }
    return target_hero;
}

EnemySpawner::EnemySpawner(
    const Vec2f& _position, 
    const Camera& _camera, 
    const PlayerManager& _player_manager,
    EnemyManager& _enemy_manager,
    doubly_linked_list<Projectile*>& projectile_manager, 
    Spawner_Data _spawner_data) 
: Enemy(_position, _camera, _player_manager, projectile_manager, E_Spawner)
, spawner_data(_spawner_data), enemy_manger(_enemy_manager), time_since_last_spawn(spawner_data.spawn_time) 
{
    is_pushable = false;
}

void EnemySpawner::update(float delta_time) {
    Enemy::update(delta_time);
    
    time_since_last_spawn += delta_time;
    if (time_since_last_spawn >= spawner_data.spawn_time && num_of_enemies < spawner_data.max_enemies) {
        spawn_enemy();
        time_since_last_spawn = 0;
    }
}

void EnemySpawner::decrement_num_of_enemies() {
    num_of_enemies--;
}

void EnemySpawner::spawn_enemy() {
    if (spawner_data.spawning_entity_.projectile_rate_of_fire == 0) {
        MovingEnemy* new_enemy = new MovingEnemy(
            get_position(), 
            camera, 
            player_manager, 
            projectile_manager_, 
            spawner_data.spawning_entity_,
            this);
        enemy_manger.add(new_enemy);
    }
    else {
        ProjectileFiringEnemy* new_enemy = new ProjectileFiringEnemy(
            get_position(), 
            camera, 
            player_manager, 
            projectile_manager_, 
            spawner_data.spawning_entity_,
            this);
        enemy_manger.add(new_enemy);
    }
    num_of_enemies++;
}

MovingEnemy::MovingEnemy(
    const Vec2f& _position, 
    const Camera& _camera, 
    const PlayerManager& _player_manager,
    doubly_linked_list<Projectile*>& projectile_manager, 
    Entity_Data entity_data,
    EnemySpawner* _owner = nullptr) 
: Enemy(_position, _camera, _player_manager, projectile_manager, entity_data)
, owner(_owner){}

void MovingEnemy::move(float delta_time, Player* target) {
    if (target == nullptr) {return;}
    
    Vec2f direction = (target->get_position() - get_position()).normalize();
    set_position(get_position() + direction * delta_time * entity_data_.speed);
}

void MovingEnemy::update(float delta_time) {
    Player* target = find_target_hero();
        
    move(delta_time, target);
        
    Enemy::update(delta_time);
}

void MovingEnemy::defeat() {
    if (owner != nullptr) {
        owner->decrement_num_of_enemies();
    }
    Enemy::defeat();
}

ProjectileFiringEnemy::ProjectileFiringEnemy(
    const Vec2f& _position, 
    const Camera& _camera,
    const PlayerManager& _player_manager, 
    doubly_linked_list<Projectile*>& projectile_manager, 
    Entity_Data entity_data,
    EnemySpawner* _owner) 
: MovingEnemy(_position, _camera, _player_manager, projectile_manager, entity_data, _owner)
{
    
}

void ProjectileFiringEnemy::try_fire_projectile(const Vec2f& direction, const float& delta_time) {
    if (time_since_projectile >= entity_data_.projectile_rate_of_fire) {
        create_projectile(PLAYER, direction);
        time_since_projectile = 0;
    } else {
        time_since_projectile += delta_time;
    }
}

void ProjectileFiringEnemy::move(float delta_time, Player* target) {
    if (target == nullptr) {return;}
    
    Vec2f direction = (target->get_position() - get_position()).normalize();
    set_position(get_position() + direction * delta_time * entity_data_.speed);
    
    try_fire_projectile(direction, delta_time);
}

