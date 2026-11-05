#pragma once
#include "Enemys.h"
#include "Players.h"

class Collision_Manager
{
protected:
    PlayerManager& player_manager_;
    EnemyManager& enemy_manager_;
    doubly_linked_list<Projectile*>& projectile_list_;
    TileMap& tile_map_;
    
public:
    Collision_Manager(PlayerManager& player_manager, EnemyManager& enemy_manager, doubly_linked_list<Projectile*>& projectile_list, TileMap& tile_map);
    
protected:
    void manage_player_collision(const float& delta_time) const;
    void manage_enemy_collision() const;
    
public:
    void manager_collision(const float& delta_time) const;
};
