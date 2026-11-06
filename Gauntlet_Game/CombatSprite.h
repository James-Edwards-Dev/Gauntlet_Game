#pragma once
#include "Collision.h"
#include "Data_Structs.h"
#include "Entity_Data.h"
#include "GameEntities.h"
#include "Projectile.h"

class CombatSprite : public WorldSpriteEntity
{
protected:
    float health;
    bool alive = true;
    
    Entity_Data entity_data_;
    Rect_Collider collider;
    
    doubly_linked_list<Projectile*>& projectile_manager_;
    
public:
    CombatSprite(const Vec2f& _position, const Camera& _camera, doubly_linked_list<Projectile*>& projectile_manager, Entity_Data entity_data);
    
    virtual void update(float delta_time) override;
    virtual void draw(GamesEngineeringBase::Window& window) override;
    virtual void defeat() = 0;
    
    const Rect_Collider& get_collider() const;
    bool is_alive() const;
    
protected:
    void create_projectile(const Target_Type target, const Vec2f& direction) const;
public:
    void damage(const float damage);
    void heal(const float heal);
    
    void apply_melee_damage(CombatSprite& other_entity, float delta_time) const ;
};
