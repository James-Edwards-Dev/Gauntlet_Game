#include "CombatEntity.h"
#include "GameEntities.h"

CombatEntity::CombatEntity(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, Entity_Data entity_data) 
: CollisionSprite( _position, entity_data.sprite_img_filepath, _camera),  
projectile_manager_(projectile_manager),
entity_data_(entity_data), 
health(entity_data.max_health)
{
    
}

void CombatEntity::draw(GamesEngineeringBase::Window& window) {
    if (alive) {
        WorldSpriteEntity::draw(window);
    }
}

bool CombatEntity::is_alive() const {
    return alive;
}

Projectile* CombatEntity::create_projectile(const Target_Type target, const Vec2f& direction) const {
    Projectile* new_projectile = new Projectile(
        get_centre(), 
        entity_data_.projectile_damage, 
        entity_data_.projectile_speed, 
        direction, 
        target, 
        camera);
    
    projectile_manager_.append(new_projectile);
    
    return new_projectile;
}

void CombatEntity::damage(const float damage) {
    // Uses defense - 100 to take a percentage of damage
    // For example if defense is, the entity will take 75% of damage
    float applied_damage = ((100 - entity_data_.defense) / 100) * damage;
    health -= applied_damage;
    
    if (health <= 0.0f) {
        defeat();
    }
}

void CombatEntity::heal(const float heal) {
    health += heal;
    if (health >= entity_data_.max_health) { health = entity_data_.max_health; }
    std::cout << "Health: " << health << "\n";
}

void CombatEntity::apply_melee_damage(CombatEntity& other_entity, float delta_time) const {
    other_entity.damage(entity_data_.melee_damage * delta_time);
}
