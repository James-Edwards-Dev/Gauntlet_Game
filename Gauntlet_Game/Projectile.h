#pragma once
#include "Collision.h"
#include "GameEntities.h"
#include "Camera.h"

class Player;
class EnemyManager;
class CombatEntity;

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
    Player* owning_hero = nullptr;
    
    Projectile(Vec2f position, const float damage, const float speed, Vec2f direction, Target_Type target, const Camera& camera_, std::string image_filename = "resources/projectile.png"); 
    
    const Rect_Collider& get_collider() const;
    
    virtual void update(float delta_time);
    
    void draw_collider(GamesEngineeringBase::Window& window) const;
    
    virtual void hit(CombatEntity& entity) const;
    
    const Target_Type& get_target_type() const;
};

class WizardAbility : public Projectile
{
    EnemyManager& enemy_manager;
    static constexpr int n_affected_enemies = 4;
    
public:
    WizardAbility(const Vec2f& position, const Vec2f& direction, const Camera& camera_, EnemyManager& enemy_manager_);
    
    virtual void hit(CombatEntity& entity) const override;
};