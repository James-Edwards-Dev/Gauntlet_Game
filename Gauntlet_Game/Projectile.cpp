#include "Projectile.h"
#include "CombatEntity.h"
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

void Projectile::hit(CombatEntity& entity) const {
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
void WizardAbility::hit(CombatEntity& entity) const {
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
    // We will loop through the enemy list n times
    // Defeating the closest enemy each time
    // When they are defeated they are marked at as not alive, so they cannot get chosen twice
    for (unsigned int i = 0; i < n_affected_enemies; i++) {
        doubly_node<Enemy*>* enemy_node = enemy_manager.Enemys_List.head;
        float min_distance = std::numeric_limits<float>::max();
        Enemy* min_distance_enemy = nullptr;
        
        while (enemy_node != nullptr) {
            doubly_node<Enemy*>* next_node = enemy_node->next;
            Enemy* enemy = enemy_node->data;
            
            // Ignore if not alive
            if (!enemy->is_alive()) {
                enemy_node = next_node;
                continue;
            }
            
            // Check if closer than current closest enemy
            float distance_squared = get_centre().distanceSquared(enemy->get_centre());
            if (distance_squared < min_distance) {
                min_distance_enemy = enemy;
                min_distance = distance_squared;
            }
            enemy_node = next_node;
        }
        if (min_distance_enemy != nullptr) {
            min_distance_enemy->defeat();
            owning_hero->gameplay_stats.enemies_defeated++;
        }
    }
}
