#pragma once

#include "GameEntities.h"
#include "Camera.h"

class HeroEntity : public CombatSprite
{
public:
    HeroEntity(const Vec2f& _position, const Camera& _camera, const std::string& image_filename, Entity_Data entity_data) 
    : CombatSprite(_position, image_filename, _camera,entity_data) {}
    
    void set_input_axis(const Vec2f& input) {inputAxis = input;}
    const Vec2f& get_input_axis() const {return inputAxis;}
    
    virtual void update(float delta_time) override {
        move(get_input_axis() * entity_data_.speed * delta_time);
        
        CombatSprite::update(delta_time);
    }
    
protected:
    Vec2f inputAxis{0.0f, 0.0f};
};

class PlayerKnight : public HeroEntity
{
public:
    PlayerKnight(const Vec2f& _position, const Camera& _camera) 
    : HeroEntity(_position, _camera, "resources/IdleAnimation2.png", Entity_Data(P_Knight)) {}
    
};

class PlayerRogue : public HeroEntity
{
public:
    PlayerRogue(const Vec2f& _position, const Camera& _camera) 
    : HeroEntity(_position, _camera, "resources/IdleAnimation2.png", Entity_Data(P_Rogue)) {}
    
};

class PlayerWizard : public HeroEntity
{
public:
    PlayerWizard(const Vec2f& _position, const Camera& _camera) 
    : HeroEntity(_position, _camera, "resources/IdleAnimation2.png", Entity_Data(P_Wizard)) {}
    
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
