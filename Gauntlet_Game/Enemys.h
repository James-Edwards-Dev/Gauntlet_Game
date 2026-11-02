#pragma once

#include "GameEntities.h"
#include "Managers.h"

class Enemy : public CombatSprite
{
protected:
    const PlayerManager player_manager;
public:
    Enemy(
        const Vec2f& _position, 
        const std::string& image_filename, 
        const Camera& _camera, 
        const PlayerManager& _player_manager,
        const float _max_health = 100, 
        const float _speed = 100, 
        const float _defense = 0) 
    : CombatSprite(_position, image_filename, _camera, _max_health, _speed, _defense), player_manager(_player_manager){}
    
    virtual void update(float delta_time) override {
        HeroEntity* target = find_closest_hero();
        
        move(delta_time, target);
        
        CombatSprite::update(delta_time);
    }
    virtual void move(float delta_time, HeroEntity* target)  {
        if (target == nullptr) {return;}
        Vec2f direction = (target->get_position() - get_position()).normalize();
        set_position(get_position() + direction * delta_time * speed) ;
    }
    
private:
    // Returns the distance to a closest target
    HeroEntity* find_closest_hero() const {
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
};
