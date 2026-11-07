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
        
        if (!player->is_alive()) continue;    
        
        // Player collision with enemies
        for (doubly_node<Enemy*>* node = enemy_manager_.Enemys_List.head; node != nullptr; node = node->next) {
            Enemy* enemy = node->data;
            
            if (enemy->get_collider().is_overlapping(player->get_collider())) {
                if (enemy->get_is_pushable()) {
                    // Push player & enemy away from each-other equally
                    enemy->get_collider().equal_block(*enemy, *player, player->get_collider());
                }
                else {
                    // Block player
                    enemy->get_collider().block_collide(*player, player->get_collider());
                }
                // Appy melee damage
                enemy->apply_melee_damage(*player, delta_time);
                player->apply_melee_damage(*enemy, delta_time);
            }
        }
        
        // Player collision with projectiles
        doubly_node<Projectile*>* projectile_node = projectile_list_.head;
        while (projectile_node != nullptr) {
            // We are getting the next node now instead of at the end of each iteration. As if a node is removed node->next is accessing a nullptr
            doubly_node<Projectile*>* next_projectile_node = projectile_node->next;
            
            Projectile* projectile = projectile_node->data;
            
            if (projectile->get_target_type() == PLAYER && projectile->get_collider().is_overlapping(player->get_collider())) {
                projectile->hit(*player);
                projectile_list_.remove_and_delete_data(projectile_node);
            }
            projectile_node = next_projectile_node;
        }
        
        // Player collision with tile map
        tile_map_.block(*player, player->get_collider());
    }
}

// Manage enemy collision with Tilemap, Projectiles & other enemies
// Collision with player is handled by 'manage_player_collision' function
void Collision_Manager::manage_enemy_collision() const {
    for (doubly_node<Enemy*>* enemy_node = enemy_manager_.Enemys_List.head; enemy_node != nullptr; enemy_node = enemy_node->next) {
        Enemy* enemy = enemy_node->data;
        
        // Enemy collision with other enemies
        // Starting from node->next to avoid checking collision with self and already checked enemies
        if (enemy->get_is_pushable()) {
            for (doubly_node<Enemy*>* other_enemy_node = enemy_node->next; other_enemy_node != nullptr; other_enemy_node = other_enemy_node->next) {
                Enemy* other_enemy = other_enemy_node->data;
                if (other_enemy->get_is_pushable() && enemy->get_collider().is_overlapping(other_enemy->get_collider())) {
                    enemy->get_collider().equal_block(*enemy, *other_enemy, other_enemy->get_collider());
                }
            }
        }
        // Enemy collision with projectiles
        doubly_node<Projectile*>* projectile_node = projectile_list_.head;
        while (projectile_node != nullptr) {
            // We are getting the next node now instead of at the end of each iteration. As if a node is removed node->next is accessing a nullptr
            doubly_node<Projectile*>* next_projectile_node = projectile_node->next; 
            
            Projectile* projectile = projectile_node->data;
            
            if (projectile->get_target_type() == ENEMY && projectile->get_collider().is_overlapping(enemy->get_collider())) {
                projectile->hit(*enemy);
                projectile_list_.remove_and_delete_data(projectile_node);
            }
            projectile_node = next_projectile_node;
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
