#pragma once

#include "Camera.h"
#include "CombatSprite.h"

class HeroEntity : public CombatSprite
{
    float time_since_last_projectile = entity_data_.projectile_rate_of_fire;
    Vec2f projectile_spawn_direction = Vec2f{1.0f, 0.0f};
    
public:
    HeroEntity(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, Entity_Data entity_data) 
    : CombatSprite(_position, _camera, projectile_manager, entity_data) {}
    
    void set_input_axis(const Vec2f& input) {
        inputAxis = input.normalize();
        if (input.x != 0 || input.y != 0) {
            projectile_spawn_direction = inputAxis;
        }
    }
    const Vec2f& get_input_axis() const {return inputAxis;}
    
    virtual void update(float delta_time) override {
        move(get_input_axis() * entity_data_.speed * delta_time);
        time_since_last_projectile += delta_time;
        
        CombatSprite::update(delta_time);
    }
    
    void fire_projectile() {
        if (time_since_last_projectile >= entity_data_.projectile_rate_of_fire) {
            create_projectile(ENEMY, projectile_spawn_direction);
            time_since_last_projectile = 0;
        }
    }
    
protected:
    Vec2f inputAxis{0.0f, 0.0f};
};

class PlayerWarrior : public HeroEntity
{
public:
    PlayerWarrior(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager) 
    : HeroEntity(_position, _camera, projectile_manager, Entity_Data(P_Warrior)) {}
    
};

class PlayerRogue : public HeroEntity
{
public:
    PlayerRogue(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager) 
    : HeroEntity(_position, _camera, projectile_manager, Entity_Data(P_Rogue)) {}
    
};

class PlayerWizard : public HeroEntity
{
public:
    PlayerWizard(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager) 
    : HeroEntity(_position, _camera, projectile_manager, Entity_Data(P_Wizard)) {}
    
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
