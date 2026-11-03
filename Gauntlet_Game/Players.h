#pragma once
#include "Data_Structs.h"
#include "Enemys.h"
#include "GameEntities.h"

class HeroEntity : public CombatSprite
{
public:
    HeroEntity(const Vec2f& _position, const Camera& _camera, const float _max_health = 100, const float _melee_damage = 50, const float _speed = 100, const float _defense = 0) 
    : CombatSprite(_position, "resources/IdleAnimation2.png", _camera, _max_health, _melee_damage, _speed, _defense) {}
    
    void set_input_axis(const Vec2f& input) {inputAxis = input;}
    const Vec2f& get_input_axis() const {return inputAxis;}
    
    virtual void update(float delta_time) override {
        move(get_input_axis() * speed * delta_time);
        
        CombatSprite::update(delta_time);
    }
    
protected:
    Vec2f inputAxis{0.0f, 0.0f};
};

class PlayerManager
{
    static constexpr unsigned int max_players = 4;
    unsigned int player_count = 0;
    
public:
    HeroEntity* players[max_players]{};
    
    bool add(HeroEntity* player) {
        if (player_count >= max_players) {return false;}
        players[player_count] = player;
        player_count++;
        
        return true;
    }
    
    unsigned int get_player_count() const { return player_count; }
    unsigned int get_max_players() const { return max_players; }
};
