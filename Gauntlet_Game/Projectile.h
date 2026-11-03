#pragma once
#include "Collision.h"
#include "GameEntities.h"
#include "Camera.h"

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
    Projectile(Vec2f position, const float damage, const float speed, Vec2f direction, Target_Type target, const Camera& camera_) : 
    WorldSpriteEntity(position, "resources/projectile.png", camera_), 
    hit_damage_(damage), speed_(speed), direction_(direction), target_type_(target), 
    collider_(get_image().width - 8, get_image().height - 20, position + Vec2f(4, 10), camera_) 
    {
        
    }
    const Rect_Collider& get_collider() const {return collider_;}
    
    virtual void update(float delta_time) override {
        move(direction_ * speed_ * delta_time);
        
        collider_.set_position(get_position());
    }
    void draw_collider(GamesEngineeringBase::Window& window) const {
        GERenderer::render_rect_outline(window, collider_.get_width(), collider_.get_height(), collider_.get_position() - camera.camera_offset);
    }
};