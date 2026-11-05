#pragma once
#include "Collision.h"
#include "Entity_Data.h"
#include "GameEntities.h"

class CombatSprite : public WorldSpriteEntity
{
protected:
    float health;
    bool alive = true;
    
    Entity_Data entity_data_;
    Rect_Collider collider;
    
public:
    CombatSprite(const Vec2f& _position, const Camera& _camera, Entity_Data entity_data);
    
    virtual void update(float delta_time) override;
    
    const Rect_Collider& get_collider() const;
    bool is_alive() const;
    
    void damage(const float damage);
    void heal(const float heal);
    
    void apply_melee_damage(CombatSprite& other_entity, float delta_time) const ;
};
