#include "Enemy.h"
#include "Enemy_Manager.h"

Enemy::Enemy(
    const Vec2f& _position, 
    const Camera& _camera, 
    const PlayerManager& _player_manager,
    doubly_linked_list<Projectile*>& projectile_manager, 
    Entity_Data entity_data) 
: CombatSprite(_position, _camera, projectile_manager, entity_data), player_manager(_player_manager){}

void Enemy::defeat() {
    alive = false;
}

bool Enemy::get_is_pushable() const {
    return is_pushable;
}

HeroEntity* Enemy::find_closest_hero() const {
    float closest_player_distance = std::numeric_limits<float>::max();
    HeroEntity* closest_player = nullptr;
        
    for (unsigned int i = 0; i < player_manager.get_player_count(); i++) {
        if (player_manager.players[i] == nullptr || !player_manager.players[i]->is_alive()) {continue;}
            
        float distance = get_centre().distanceSquared(player_manager.players[i]->get_centre());
        if (distance < abs(closest_player_distance)) {
            closest_player = player_manager.players[i];
            closest_player_distance = distance;
        }
    }
    return closest_player;
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
    if (time_since_last_spawn >= spawner_data.spawn_time) {
        spawn_enemy();
        time_since_last_spawn = 0;
    }
}

void EnemySpawner::spawn_enemy() {
    MovingEnemy* new_enemy = new MovingEnemy(
        get_position(), 
        camera, 
        player_manager, 
        projectile_manager_, 
        spawner_data.spawning_entity_);
    enemy_manger.add(new_enemy);
}

MovingEnemy::MovingEnemy(
    const Vec2f& _position, 
    const Camera& _camera, 
    const PlayerManager& _player_manager,
    doubly_linked_list<Projectile*>& projectile_manager, 
    Entity_Data entity_data) 
: Enemy(_position, _camera, _player_manager, projectile_manager, entity_data) {}

void MovingEnemy::move(float delta_time, HeroEntity* target) {
    if (target == nullptr) {return;}
    
    Vec2f direction = (target->get_position() - get_position()).normalize();
    set_position(get_position() + direction * delta_time * entity_data_.speed);
}

void MovingEnemy::update(float delta_time) {
    HeroEntity* target = find_closest_hero();
        
    move(delta_time, target);
        
    Enemy::update(delta_time);
}

