#pragma once

#include "GameEntities.h"
#include "Camera.h"

class HeroEntity : public CombatSprite
{
public:
    HeroEntity(const Vec2f& _position, const Camera& _camera, const std::string& image_filename, 
        const float _melee_damage = 50, 
        const float _projectile_damage = 50, 
        const float _defense = 0, 
        const float _speed = 60, 
        const float _max_health = 100) 
    : CombatSprite(_position, image_filename, _camera, _max_health, _melee_damage, _projectile_damage, _speed, _defense) {}
    
    void set_input_axis(const Vec2f& input) {inputAxis = input;}
    const Vec2f& get_input_axis() const {return inputAxis;}
    
    virtual void update(float delta_time) override {
        move(get_input_axis() * speed * delta_time);
        
        CombatSprite::update(delta_time);
    }
    
protected:
    Vec2f inputAxis{0.0f, 0.0f};
};

class PlayerKnight : public HeroEntity
{
public:
    PlayerKnight(const Vec2f& _position, const Camera& _camera) 
    : HeroEntity(_position, _camera, "resources/IdleAnimation2.png", 100, 50, 40, 60) {}
    
};

class PlayerRogue : public HeroEntity
{
public:
    PlayerRogue(const Vec2f& _position, const Camera& _camera) 
    : HeroEntity(_position, _camera, "resources/IdleAnimation2.png", 50, 70, 10, 120) {}
    
};

class PlayerWizard : public HeroEntity
{
public:
    PlayerWizard(const Vec2f& _position, const Camera& _camera) 
    : HeroEntity(_position, _camera, "resources/IdleAnimation2.png", 30, 100, 0, 75) {}
    
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
    
    void update(float delta_time, FollowCamera& camera) const {
        if (player_count > 0) {
            Vec2f* player_locations = new Vec2f[player_count];
            for (unsigned int i = 0; i < player_count; i++) {
                player_locations[i] = players[i]->get_position();
            }
            camera.update(delta_time, player_locations, player_count);
        }
    }
    
    void reset_camera(FollowCamera& camera) const {
        if (player_count <= 0) return;
        
        Vec2f* player_locations = new Vec2f[player_count];
        for (unsigned int i = 0; i < player_count; i++) {
            player_locations[i] = players[i]->get_position();
        }
        camera.snap_to_target_location(player_locations, player_count);
    }
};
