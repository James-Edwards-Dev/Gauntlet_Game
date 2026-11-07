#include "Enemy.h"

Enemy::Enemy(
    const Vec2f& _position, 
    const Camera& _camera, 
    const PlayerManager& _player_manager,
    doubly_linked_list<Projectile*>& projectile_manager, 
    Entity_Data entity_data) 
: CombatSprite(_position, _camera, projectile_manager, entity_data), player_manager(_player_manager){}

void Enemy::update(float delta_time) {
    HeroEntity* target = find_closest_hero();
        
    move(delta_time, target);
        
    CombatSprite::update(delta_time);
}

void Enemy::defeat() {
    alive = false;
}

void Enemy::move(float delta_time, HeroEntity* target) {
    if (target == nullptr) {return;}
    
    Vec2f direction = (target->get_position() - get_position()).normalize();
    set_position(get_position() + direction * delta_time * entity_data_.speed);
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

