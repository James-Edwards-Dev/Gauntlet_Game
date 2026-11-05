#include "Projectile.h"
#include "CombatSprite.h"

Projectile::Projectile(Vec2f position, const float damage, const float speed, Vec2f direction, Target_Type target, const Camera& camera_)
: WorldSpriteEntity(position, "resources/projectile.png", camera_), 
hit_damage_(damage), 
speed_(speed), 
direction_(direction), 
target_type_(target), 
collider_(get_image().width - 8, get_image().height - 20, position + Vec2f(4, 10), camera_) 
{
    
}

const Rect_Collider& Projectile::get_collider() const { return collider_; }

void Projectile::update(float delta_time) {
    move(direction_ * speed_ * delta_time);
        
    collider_.set_position(get_position());
}

void Projectile::draw_collider(GamesEngineeringBase::Window& window) const {
    GERenderer::render_rect_outline(window, collider_.get_width(), collider_.get_height(), collider_.get_position() - camera.camera_offset);
}

void Projectile::hit(CombatSprite& entity) const {
    entity.damage(hit_damage_);
}

const Target_Type& Projectile::get_target_type() const {
    return target_type_;
}
