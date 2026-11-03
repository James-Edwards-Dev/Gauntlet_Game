#pragma once

#include "GameEntities.h"
#include "Players.h"
#include "TileMap.h"

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
        const float _speed = 80, 
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

class EnemyManager
{
    doubly_linked_list<Enemy*> Enemys_List;
    
public:
    void add(Enemy* enemy) {
        Enemys_List.append(enemy);
    }
    void update(float delta_time) {
        for (doubly_node<Enemy*>* node = Enemys_List.head; node != nullptr; node = node->next) {
            Enemy* enemy = node->data;
            enemy->update(delta_time);
        }
    }
    void draw(GamesEngineeringBase::Window& window) const {
        for (doubly_node<Enemy*>* node = Enemys_List.head; node != nullptr; node = node->next) {
            Enemy* enemy = node->data;
            enemy->draw(window);
        }
    }
    void draw_colliders(GamesEngineeringBase::Window& window) const {
        for (doubly_node<Enemy*>* node = Enemys_List.head; node != nullptr; node = node->next) {
            Enemy* enemy = node->data;
            enemy->get_collider().draw_debug_lines(window);
        }
    }
    void check_collision(TileMap& tile_map, PlayerManager& players) {
        for (doubly_node<Enemy*>* node = Enemys_List.head; node != nullptr; node = node->next) 
        {
            Enemy* enemy = node->data;
            Rect_Collider enemy_collider = enemy->get_collider();
            
            // Collision with player
            for (unsigned int i = 0; i < players.get_player_count(); i++) {
                Rect_Collider player_collider = players.players[i]->get_collider();
                if (enemy_collider.is_overlapping(player_collider)) {
                    player_collider.push_collide(*enemy, enemy_collider);
                    
                    std::cout << "Overlapping player " << i + 1 << "\n";
                }
            }
            // Collision with other enemies
            // Starting from node->next all previous nodes have already been checked
            for (doubly_node<Enemy*>* other_node = node->next; other_node != nullptr; other_node = other_node->next) {
                Enemy* other_enemy = other_node->data;
                Rect_Collider other_collider = other_enemy->get_collider();
                if (enemy_collider.is_overlapping(other_collider)) {
                    enemy_collider.push_collide(*other_enemy, other_collider);
                }
            }
            
            tile_map.check_collision(*enemy, enemy_collider);
        }
    }
};
