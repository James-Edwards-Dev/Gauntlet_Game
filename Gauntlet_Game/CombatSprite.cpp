#include "CombatSprite.h"
#include "GameEntities.h"

CombatSprite::CombatSprite(const Vec2f& _position, const Camera& _camera, Entity_Data entity_data) 
: WorldSpriteEntity( _position, entity_data.sprite_img_filepath, _camera),  
entity_data_(entity_data), health(entity_data.max_health), collider(get_image().width, get_image().height, _position, _camera) 
{
    
}

void CombatSprite::update(float delta_time) {
    // Moving collider
    collider.set_position(get_position());
}

const Rect_Collider& CombatSprite::get_collider() const {
    return collider;
}

bool CombatSprite::is_alive() const {
    return alive;
}

void CombatSprite::damage(const float damage) {
    // Uses defense - 100 to take a percentage of damage
    // For example if defense is, the entity will take 75% of damage
    float applied_damage = ((100 - entity_data_.defense) / 100) * damage;
    health -= applied_damage;
        
    std::cout << "Health: " << health << "\n";
    if (health <= 0.0f) {std::cout << "You died\n";}
}

void CombatSprite::heal(const float heal) {
    health += heal;
    if (health >= entity_data_.max_health) { health = entity_data_.max_health; }
    std::cout << "Health: " << health << "\n";
}

void CombatSprite::apply_melee_damage(CombatSprite& other_entity, float delta_time) const {
    other_entity.damage(entity_data_.melee_damage * delta_time);
}
