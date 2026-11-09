#pragma once

#include "Camera.h"
#include "CombatEntity.h"

struct Gameplay_Stats
{
    float alive_time = 0;
    unsigned int enemies_defeated = 0;
    bool victory = false;
    unsigned int player_num;
    unsigned int hero_type = 0;
    
    void display() const {
        std::cout << "Alive Time:       " << static_cast<int>(alive_time) << std::endl;
        std::cout << "Enemies Defeated: " << enemies_defeated << std::endl;
        if (victory) {
            std::cout << "Victorious" << std::endl;
        } else {
            std::cout << "Loss" << std::endl;
        }
    }
    
    void draw(const Vec2f& location, GamesEngineeringBase::Window& window, const GamesEngineeringBase::Image& text_image) const {
        Vec2f row_size = {0, 32.0f};
        std::string tile_string = "Player " + std::to_string(player_num);
        std::string str_alive_time = "Alive Time:        " + std::to_string(static_cast<int>(alive_time));
        std::string str_enemies_defeated = "Enemies Defeated:  " + std::to_string(enemies_defeated);
        std::string result_message;
        if (victory) {
            result_message = "Victorious!";
        } else {
            result_message = "You Lost!";
        }
        GERenderer::render_string(window, text_image, tile_string, location - row_size * 3);
        GERenderer::render_string(window, text_image, str_alive_time, location);
        GERenderer::render_string(window, text_image, str_enemies_defeated, location + row_size);
        GERenderer::render_string(window, text_image, result_message, location + (row_size * 3));
    }
};

// This is used to change enemy targeting behavior
// If player is low priority they are less likely to be targeted by enemies
// If they are high priority they are more likely to be targeted by enemies
enum Attack_Priority
{
    LOW_PRIORITY,
    NORMAL_PRIORITY,
    HIGH_PRIORITY
};

class Player : public CombatEntity
{
protected:
    float time_since_last_projectile = entity_data_.projectile_rate_of_fire;
    Vec2f projectile_spawn_direction = Vec2f{1.0f, 0.0f};
private:
    int up_key;
    int down_key;
    int left_key;
    int right_key;
    int projectile_key;
    int abilty_key;
public:
    Gameplay_Stats gameplay_stats;
    
protected:
    Attack_Priority  enemy_attack_priority = NORMAL_PRIORITY;
    
    float ability_up_time = 8.0f;
    float ability_cooldown_time = 8.0f;
    
    bool ability_active = false;
    float current_ability_up_time = 0;
    float time_since_last_ability = ability_cooldown_time;
    
public:
    Player(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, Entity_Data entity_data, unsigned int player_number) 
    : CombatEntity(_position, _camera, projectile_manager, entity_data) 
    {
        gameplay_stats.player_num = player_number;
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
    
    virtual void use_ability() =0;
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
            use_ability();
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
            gameplay_stats.alive_time += delta_time;
            
            CombatEntity::update(delta_time);
        }
    }
    
    virtual void defeat() override {
        alive = false;
    }
    
    void fire_projectile() {
        if (time_since_last_projectile >= entity_data_.projectile_rate_of_fire) {
            Projectile* projectile = create_projectile(ENEMY, projectile_spawn_direction);
            projectile->owning_hero = this;
            time_since_last_projectile = 0;
        }
    }
    
protected:
    Vec2f inputAxis{0.0f, 0.0f};
};

class PlayerWarrior : public Player
{
    float standard_defence = entity_data_.defense;
    float increased_defence = 80; // Defence when ability is active
    float standard_melee_damage = entity_data_.melee_damage;
    float increased_melee_damage = 200; // Melee damage when ability is active
    float standard_speed = entity_data_.speed;
    float decreased_speed = 45; // speed when ability is active
    
public:
    PlayerWarrior(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, unsigned int player_number) 
    : Player(_position, _camera, projectile_manager, Entity_Data(P_Warrior), player_number) {}
    
    virtual void use_ability() override {
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

class PlayerRogue : public Player
{
    float standard_speed = entity_data_.speed;
    float increased_speed = 250.0f; // speed when ability is active
    
public:
    PlayerRogue(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, unsigned int player_number) 
    : Player(_position, _camera, projectile_manager, Entity_Data(P_Rogue), player_number) {}
    
    virtual void use_ability() override {
        entity_data_.speed = increased_speed;
        enemy_attack_priority = LOW_PRIORITY;
        current_ability_up_time = 0;
        ability_active = true;
    }
    virtual void update_ability(float delta_time) override {
        if (current_ability_up_time >= ability_up_time) {
            entity_data_.speed = standard_speed;
            enemy_attack_priority = NORMAL_PRIORITY;
            ability_active = false;
        }
        else {
            current_ability_up_time += delta_time;
        }
    }
};

class PlayerWizard : public Player
{
    EnemyManager& enemy_manager_;
public:
    PlayerWizard(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, EnemyManager& enemy_manager, unsigned int player_number) 
    : Player(_position, _camera, projectile_manager, Entity_Data(P_Wizard), player_number), enemy_manager_(enemy_manager) {}
    
    virtual void use_ability() override {
        if (time_since_last_ability >= ability_cooldown_time) {
            WizardAbility* ability = new WizardAbility(
                get_centre(),
                projectile_spawn_direction,
                camera,
                enemy_manager_);
            ability->owning_hero = this;
            projectile_manager_.append(ability);
            time_since_last_ability = 0;
        }
    }
    
    virtual void update_ability(float delta_time) override {
        if (time_since_last_ability < ability_cooldown_time) {
            time_since_last_ability += delta_time;
        }
    }
};


class PlayerManager
{
    static constexpr unsigned int max_players = 4;
    unsigned int player_count = 0;
    
    unsigned int alive_players = 0;
    
    bool should_game_end_ = false;
    
public:
    Player* players[max_players]{};
    
    bool add(Player* player) {
        if (player_count >= max_players) {return false;}
        players[player_count] = player;
        player_count++;
        alive_players++;
        
        return true;
    }
    void create_players(int characters[], unsigned int num_of_characters, Camera& camera, doubly_linked_list<Projectile*>& projectile_manager, EnemyManager& enemy_manager) {
        for (unsigned int i = 0; i < num_of_characters; i++) {
            Vec2f spawn_pos = Vec2f{32.0f, 64.0f} + (Vec2f{32.0f, 0} * i);
            if (characters[i] == 0) {
                PlayerWarrior* new_player = new PlayerWarrior(spawn_pos, camera, projectile_manager, i + 1);
                new_player->gameplay_stats.hero_type = characters[i];
                add(new_player);
            } else if (characters[i] == 1) {
                PlayerRogue* new_player = new PlayerRogue(spawn_pos, camera, projectile_manager, i + 1);
                add(new_player);
                new_player->gameplay_stats.hero_type = characters[i];
            } else {
                PlayerWizard* new_player = new PlayerWizard(spawn_pos, camera, projectile_manager, enemy_manager, i + 1);
                add(new_player);
                new_player->gameplay_stats.hero_type = characters[i];
            }
        }
    }
    
    unsigned int get_player_count() const { return player_count; }
    unsigned int get_max_players() const { return max_players; }
    unsigned int get_alive_players() const { return alive_players; }
    bool should_game_end() const { return should_game_end_; }
    
    Gameplay_Stats* get_game_end_stats() const {
        Gameplay_Stats* game_end_stats = new Gameplay_Stats[4];
        for (unsigned int i = 0; i < get_player_count(); i++) {
            game_end_stats[i] = players[i]->gameplay_stats;
            game_end_stats[i].display();
        } 
        return game_end_stats;
    }
    
    void update(float delta_time, GamesEngineeringBase::Window& window, FollowCamera& camera) {
        if (player_count > 0) {
            // Saving player locations for camera update
            Vec2f* alive_player_locations = new Vec2f[player_count];
            
            should_game_end_ = true;
            alive_players = 0;
            for (unsigned int i = 0; i < player_count; i++) {
                players[i]->update_input(window);
                players[i]->update(delta_time);
                
                // Only record alive players for camera
                if (players[i]->is_alive()) {
                    alive_player_locations[alive_players] = players[i]->get_centre();
                    alive_players++;
                    if (!players[i]->gameplay_stats.victory) {
                        should_game_end_ = false;
                    }
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
