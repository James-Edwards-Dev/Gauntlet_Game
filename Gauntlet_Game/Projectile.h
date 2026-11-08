#pragma once
#include "Collision.h"
#include "GameEntities.h"
#include "Camera.h"

class HeroEntity;

class CombatSprite;

enum Target_Type
{
    PLAYER,
    ENEMY
};

class Projectile : public WorldSpriteEntity
{
    const float hit_damage_;
    const float speed_;
    const Vec2f direction_;
    
    const Target_Type target_type_;
    Rect_Collider collider_;
public:
    HeroEntity* owning_hero = nullptr;
    
    Projectile(Vec2f position, const float damage, const float speed, Vec2f direction, Target_Type target, const Camera& camera_); 
    
    const Rect_Collider& get_collider() const;
    
    virtual void update(float delta_time);
    
    void draw_collider(GamesEngineeringBase::Window& window) const;
    
    void hit(CombatSprite& entity) const;
    
    const Target_Type& get_target_type() const;
};