#include "Collision_Manager.h"

Collision_Manager::Collision_Manager(PlayerManager& player_manager, EnemyManager& enemy_manager, doubly_linked_list<Projectile*>& projectile_list, TileMap& tile_map)
    : player_manager_(player_manager), enemy_manager_(enemy_manager), projectile_list_(projectile_list), tile_map_(tile_map)
{
    
}

// Managers player collision with; Enemies, Tilemap and Projectiles
void Collision_Manager::manage_player_collision(const float& delta_time) const
{
    for (unsigned int i = 0; i < player_manager_.get_player_count(); i++) {
        HeroEntity* player = player_manager_.players[i];
        
        // Player collision with enemies
        for (doubly_node<Enemy*>* node = enemy_manager_.Enemys_List.head; node != nullptr; node = node->next) {
            Enemy* enemy = node->data;
            
            if (enemy->get_collider().is_overlapping(player->get_collider())) {
                // Push player & enemy away from each-other equally
                enemy->get_collider().equal_block(*enemy, *player, player->get_collider());
                // Appy melee damage
                enemy->apply_melee_damage(*player, delta_time);
                player->apply_melee_damage(*enemy, delta_time);
            }
        }
        
        // Player collision with projectiles
        for (doubly_node<Projectile*>* node = projectile_list_.head; node != nullptr; node = node->next) {
            Projectile* projectile = node->data;
            
            // Continue if not targeting enemy
            if (projectile->get_target_type() != PLAYER) continue;
            
            projectile->hit(*player);
        }
        
        // Player collision with tile map
        tile_map_.block(*player, player->get_collider());
    }
}

// Manage enemy collision with Tilemap, Projectiles & other enemies
// Collision with player is handled by 'manage_player_collision' function
void Collision_Manager::manage_enemy_collision() const {
    for (doubly_node<Enemy*>* node = enemy_manager_.Enemys_List.head; node != nullptr; node = node->next) {
        Enemy* enemy = node->data;
        
        // Enemy collision with other enemies
        // Starting from node->next to avoid checking collision with self and already checked enemies
        for (doubly_node<Enemy*>* other_node = node->next; other_node != nullptr; other_node = other_node->next) {
            Enemy* other_enemy = other_node->data;
            if (enemy->get_collider().is_overlapping(other_enemy->get_collider())) {
                enemy->get_collider().equal_block(*enemy, *other_enemy, other_enemy->get_collider());
            }
        }
        
        // Enemy collision with projectiles
        for (doubly_node<Projectile*>* node = projectile_list_.head; node != nullptr; node = node->next) {
            Projectile* projectile = node->data;
            
            // Continue if not targeting enemy
            if (projectile->get_target_type() != ENEMY) continue;
            
            projectile->hit(*enemy);
        }
        
        // Enemy collision with tilemap
        tile_map_.block(*enemy, enemy->get_collider());
    }
}

void Collision_Manager::manager_collision(const float& delta_time) const
{
    manage_player_collision(delta_time);
    manage_enemy_collision();
}
