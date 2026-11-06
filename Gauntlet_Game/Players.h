#pragma once

#include "Camera.h"
#include "CombatSprite.h"

class HeroEntity : public CombatSprite
{
    float time_since_last_projectile = entity_data_.projectile_rate_of_fire;
    Vec2f projectile_spawn_direction = Vec2f{1.0f, 0.0f};
    
    int up_key;
    int down_key;
    int left_key;
    int right_key;
    int projectile_key;
    
public:
    HeroEntity(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, Entity_Data entity_data, unsigned int player_number) 
    : CombatSprite(_position, _camera, projectile_manager, entity_data) 
    {
        // Set keys
        switch (player_number) {
            case 2:
                up_key = 'T';
                down_key = 'G';
                left_key = 'F';
                right_key = 'H';
                projectile_key = 'Y';
                break;
            case 3:
                up_key = VK_UP; 
                down_key = VK_DOWN;
                left_key = VK_LEFT;
                right_key = VK_RIGHT;
                projectile_key = 'M';
                break;
            default:
                up_key = 'W';
                down_key = 'S';
                left_key = 'A';
                right_key = 'D';
                projectile_key = 'E';
                break;
        }
    }
    
    void set_input_axis(const Vec2f& input) {
        inputAxis = input.normalize();
        if (input.x != 0 || input.y != 0) {
            projectile_spawn_direction = inputAxis;
        }
    }
    const Vec2f& get_input_axis() const {return inputAxis;}
    
    void update_input(GamesEngineeringBase::Window& window) {
        Vec2f input{0, 0};
        if (window.keyPressed(up_key)) {input.y = -1.0f;}
        if (window.keyPressed(down_key)) {input.y = 1.0f;}
        if (window.keyPressed(left_key)) {input.x = -1.0f;}
        if (window.keyPressed(right_key)) {input.x = 1.0f ;}
        
        set_input_axis(input);
        
        if (window.keyPressed(projectile_key)) {fire_projectile();}
    }
    
    virtual void update(float delta_time) override {
        if (alive) {
            move(get_input_axis() * entity_data_.speed * delta_time);
            time_since_last_projectile += delta_time;
                    
            CombatSprite::update(delta_time);
        }
    }
    
    virtual void defeat() override {
        alive = false;
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
    PlayerWarrior(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, unsigned int player_number) 
    : HeroEntity(_position, _camera, projectile_manager, Entity_Data(P_Warrior), player_number) {}
    
};

class PlayerRogue : public HeroEntity
{
public:
    PlayerRogue(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, unsigned int player_number) 
    : HeroEntity(_position, _camera, projectile_manager, Entity_Data(P_Rogue), player_number) {}
    
};

class PlayerWizard : public HeroEntity
{
public:
    PlayerWizard(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, unsigned int player_number) 
    : HeroEntity(_position, _camera, projectile_manager, Entity_Data(P_Wizard), player_number) {}
    
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
    
    void update(float delta_time, GamesEngineeringBase::Window& window, FollowCamera& camera) const {
        if (player_count > 0) {
            // Saving player locations for camera update
            Vec2f* player_locations = new Vec2f[player_count];
            
            unsigned int alive_players = 0;
            for (unsigned int i = 0; i < player_count; i++) {
                players[i]->update_input(window);
                players[i]->update(delta_time);
                
                // Only record alive players for camera
                if (players[i]->is_alive()) {
                    player_locations[alive_players] = players[i]->get_position();
                    alive_players++;
                }
            }
            camera.update(delta_time, player_locations, alive_players);
            delete[] player_locations;
        }
    }
    
    void draw(GamesEngineeringBase::Window& window) const {
        for (unsigned int i = 0; i < player_count; i++) {
            players[i]->draw(window);
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
