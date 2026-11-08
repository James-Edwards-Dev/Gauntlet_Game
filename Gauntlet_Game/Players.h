#pragma once

#include "Camera.h"
#include "CombatSprite.h"

// This is used to change enemy targeting behavior
// If player is low priority they are less likely to be targeted by enemies
// If they are high priority they are more likely to be targeted by enemies
enum Attack_Priority
{
    LOW_PRIORITY,
    NORMAL_PRIORITY,
    HIGH_PRIORITY
};

class HeroEntity : public CombatSprite
{
    float time_since_last_projectile = entity_data_.projectile_rate_of_fire;
    Vec2f projectile_spawn_direction = Vec2f{1.0f, 0.0f};
    
    int up_key;
    int down_key;
    int left_key;
    int right_key;
    int projectile_key;
    int abilty_key;
public:
    bool finish_line_reached = false;
    
protected:
    Attack_Priority  enemy_attack_priority = NORMAL_PRIORITY;
    
    float ability_up_time = 5.0f;
    float ability_cooldown_time = 5.0f;
    
    bool ability_active = false;
    float current_ability_up_time = 0;
    float time_since_last_ability = ability_cooldown_time;
    
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
                abilty_key = 'R';
                break;
            case 3:
                up_key = VK_UP; 
                down_key = VK_DOWN;
                left_key = VK_LEFT;
                right_key = VK_RIGHT;
                projectile_key = 'M';
                abilty_key = 'N';
                break;
            default:
                up_key = 'W';
                down_key = 'S';
                left_key = 'A';
                right_key = 'D';
                projectile_key = 'E';
                abilty_key = 'Q';
                break;
        }
    }
    
    virtual void magic_ability() =0;
    virtual void update_ability(float delta_time) =0;
    
    const Attack_Priority& get_enemy_attack_priority() const {
        return enemy_attack_priority;
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
        
        
        if (window.keyPressed(abilty_key) && time_since_last_ability >= ability_cooldown_time) {
            magic_ability();
            time_since_last_ability = 0;
        }
    }
    
    virtual void update(float delta_time) override {
        if (alive) {
            move(get_input_axis() * entity_data_.speed * delta_time);
            time_since_last_projectile += delta_time;
            
            if (ability_active) {
                update_ability(delta_time);
            }
            else if (time_since_last_ability < ability_cooldown_time) {
                time_since_last_ability += delta_time;
            }
            
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
    float standard_defence = entity_data_.defense;
    float increased_defence = 100; // Defence when ability is active
    float standard_melee_damage = entity_data_.melee_damage;
    float increased_melee_damage = 100; // Melee damage when ability is active
    float standard_speed = entity_data_.speed;
    float decreased_speed = 50; // speed when ability is active
    
public:
    PlayerWarrior(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, unsigned int player_number) 
    : HeroEntity(_position, _camera, projectile_manager, Entity_Data(P_Warrior), player_number) {}
    
    virtual void magic_ability() override {
        entity_data_.defense = increased_defence;
        entity_data_.melee_damage = increased_melee_damage;
        entity_data_.speed = decreased_speed;
        enemy_attack_priority = HIGH_PRIORITY;
        current_ability_up_time = 0;
        ability_active = true;
    }
    virtual void update_ability(float delta_time) override {
        if (current_ability_up_time >= ability_up_time) {
            entity_data_.defense = standard_defence;
            entity_data_.melee_damage = standard_melee_damage;
            entity_data_.speed = standard_speed;
            enemy_attack_priority = NORMAL_PRIORITY;
            ability_active = false;
        }
        else {
            current_ability_up_time += delta_time;
        }
    }
};

class PlayerRogue : public HeroEntity
{
    float standard_speed = entity_data_.speed;
    float increased_speed = 300.0f; // speed when ability is active
    float standard_rate_of_fire = entity_data_.projectile_rate_of_fire;
    float increased_rate_of_fire = 0.1; // speed when ability is active
    
public:
    PlayerRogue(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, unsigned int player_number) 
    : HeroEntity(_position, _camera, projectile_manager, Entity_Data(P_Rogue), player_number) {}
    
    virtual void magic_ability() override {
        entity_data_.speed = increased_speed;
        entity_data_.projectile_rate_of_fire = increased_rate_of_fire;
        enemy_attack_priority = LOW_PRIORITY;
        current_ability_up_time = 0;
        ability_active = true;
    }
    virtual void update_ability(float delta_time) override {
        if (current_ability_up_time >= ability_up_time) {
            entity_data_.speed = standard_speed;
            entity_data_.projectile_rate_of_fire = standard_rate_of_fire;
            enemy_attack_priority = NORMAL_PRIORITY;
            ability_active = false;
        }
        else {
            current_ability_up_time += delta_time;
        }
    }
};

class PlayerWizard : public HeroEntity
{
public:
    PlayerWizard(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, unsigned int player_number) 
    : HeroEntity(_position, _camera, projectile_manager, Entity_Data(P_Wizard), player_number) {}
    
    virtual void magic_ability() override {}
    virtual void update_ability(float delta_time) override {}
};


class PlayerManager
{
    static constexpr unsigned int max_players = 4;
    unsigned int player_count = 0;
    
    unsigned int alive_players = 0;
    
public:
    HeroEntity* players[max_players]{};
    
    bool add(HeroEntity* player) {
        if (player_count >= max_players) {return false;}
        players[player_count] = player;
        player_count++;
        alive_players++;
        
        return true;
    }
    void create_players(int characters[], unsigned int num_of_characters, Camera& camera, doubly_linked_list<Projectile*>& projectile_manager) {
        for (unsigned int i = 0; i < num_of_characters; i++) {
            Vec2f spawn_pos = Vec2f{32.0f, 64.0f} + (Vec2f{32.0f, 0} * i);
            if (characters[i] == 0) {
                PlayerWarrior* warrior = new PlayerWarrior(spawn_pos, camera, projectile_manager, i + 1);
                add(warrior);
            } else if (characters[i] == 1) {
                PlayerRogue* rogue = new PlayerRogue(spawn_pos, camera, projectile_manager, i + 1);
                add(rogue);
            } else {
                PlayerWizard* wizard = new PlayerWizard(spawn_pos, camera, projectile_manager, i + 1);
                add(wizard);
            }
        }
    }
    
    unsigned int get_player_count() const { return player_count; }
    unsigned int get_max_players() const { return max_players; }
    unsigned int get_alive_players() const { return alive_players; }
    
    void update(float delta_time, GamesEngineeringBase::Window& window, FollowCamera& camera) {
        if (player_count > 0) {
            // Saving player locations for camera update
            Vec2f* alive_player_locations = new Vec2f[player_count];
            
            alive_players = 0;
            for (unsigned int i = 0; i < player_count; i++) {
                players[i]->update_input(window);
                players[i]->update(delta_time);
                
                // Only record alive players for camera
                if (players[i]->is_alive()) {
                    alive_player_locations[alive_players] = players[i]->get_position();
                    alive_players++;
                }
            }
            camera.update(delta_time, alive_player_locations, alive_players);
            delete[] alive_player_locations;
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
