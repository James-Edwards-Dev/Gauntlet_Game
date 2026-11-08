#pragma once

#include "Enemy_Manager.h"
#include "Pick_Up_Item.h"
#include "Players.h"

class Collision_Manager
{
protected:
    PlayerManager& player_manager_;
    EnemyManager& enemy_manager_;
    doubly_linked_list<Projectile*>& projectile_list_;
    TileMap& tile_map_;
    doubly_linked_list<Pick_Up_Item*>& pick_up_manager_;
    
public:
    Collision_Manager(
        PlayerManager& player_manager, 
        EnemyManager& enemy_manager, 
        doubly_linked_list<Projectile*>& projectile_list, 
        TileMap& tile_map, 
        doubly_linked_list<Pick_Up_Item*>& pick_up_manager);
    
protected:
    void manage_player_collision(const float& delta_time) const;
    void manage_enemy_collision() const;
    void manage_projectile_tilemap_collision() const;
    
public:
    void manager_collision(const float& delta_time) const;
};
