#include "Projectile.h"
#include "CombatSprite.h"
#include "Enemy.h"
#include "Enemy_Manager.h"

Projectile::Projectile(Vec2f position, const float damage, const float speed, Vec2f direction, Target_Type target, const Camera& camera_, std::string image_filename)
: WorldSpriteEntity(position, image_filename, camera_), 
hit_damage_(damage), 
speed_(speed), 
direction_(direction), 
target_type_(target), 
collider_(get_image().width, get_image().height, position + Vec2f(4, 10), camera_) 
{
    
}

const Rect_Collider& Projectile::get_collider() const { return collider_; }

void Projectile::update(float delta_time) {
    move(direction_ * speed_ * delta_time);
        
    collider_.set_position(get_position());
}

void Projectile::draw_collider(GamesEngineeringBase::Window& window) const {
    get_collider().draw_debug_lines(window);
}

void Projectile::hit(CombatSprite& entity) const {
    entity.damage(hit_damage_);
}

const Target_Type& Projectile::get_target_type() const {
    return target_type_;
}

WizardAbility::WizardAbility(const Vec2f& position, const Vec2f& direction, const Camera& camera_,EnemyManager& enemy_manager_) 
: Projectile(position, 0, 80.f, direction, ENEMY, camera_, "resources/wizard_ability.png"), enemy_manager(enemy_manager_)
{
    
}

// Find the n closest enemies and to the enemy that was hit by the projectile and destroy them
// (Including the enemy that was hit originally)
void WizardAbility::hit(CombatSprite& entity) const {
    Projectile::hit(entity);
    
    // If there is n enemies or less
    // Defeat all of them in a loop and return
    if (enemy_manager.Enemys_List.size <= n_affected_enemies) {
        doubly_node<Enemy*>* enemy_node = enemy_manager.Enemys_List.head;
        while (enemy_node != nullptr) {
            doubly_node<Enemy*>* next_node = enemy_node->next;
            if (enemy_node->data->is_alive()) {
                enemy_node->data->defeat();
                owning_hero->gameplay_stats.enemies_defeated++;
            }
            enemy_node = next_node;
        }
        return;
    }
    // If there is more than n enemies. We will create a sorted array of enemies (of size n) and destroy all enemies in that array
    Enemy* n_closest_enemies[n_affected_enemies];
    float n_closest_distance_squared[n_affected_enemies];
    int found = 0;
    
    doubly_node<Enemy*>* enemy_node = enemy_manager.Enemys_List.head;
    while (enemy_node != nullptr) {
        doubly_node<Enemy*>* next_node = enemy_node->next;
        Enemy* enemy = enemy_node->data;
        
        if (!enemy->is_alive()){
            enemy_node = next_node;
            continue;
        }
        float distance_squared = get_centre().distanceSquared(enemy->get_centre());
        
        // If the array is full and the distance is larger than the furthest enemy in the array we continue 
        // If we haven't continued the current enemy will be stored in the array
        if (found == n_affected_enemies && distance_squared >= n_closest_distance_squared[n_affected_enemies - 1]){
            enemy_node = next_node;
            continue;
        }
        
        int i;
        if (found < n_affected_enemies) {
            i = found;
        } else {
            i = n_affected_enemies - 1;
        }
        // Check if the distance at i - 1 is more than the distance of this element
        // If it is shift the elements (overwriting the largest distance)
        while (i > 0 && n_closest_distance_squared[i - 1] > distance_squared) {
            n_closest_distance_squared[i] = n_closest_distance_squared[i - 1];
            n_closest_enemies[i] = n_closest_enemies[i - 1];
            i--;
        }
        // Overwrite the data stored at i with the new element
        n_closest_distance_squared[i] = distance_squared;
        n_closest_enemies[i] = enemy;
        if (found < n_affected_enemies) found++;
        
        enemy_node = next_node;
    }
    
    // Defeating the closest N enemies
    for (int i = 0; i < found; i++) {
        n_closest_enemies[i]->defeat();
        owning_hero->gameplay_stats.enemies_defeated++;
    }
}
