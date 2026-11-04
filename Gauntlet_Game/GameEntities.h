#pragma once
#include "Entity.h"
#include <iostream>

#include "GERenderer.h"
#include "Camera.h"
#include "Entity_Data.h"

class SpriteEntity : public VisualEntity
{
protected:
    GamesEngineeringBase::Image image;
public:
    SpriteEntity(const Vec2f& _position, const std::string& image_filename) : VisualEntity(_position)
    {
        image.load(image_filename);
    }
    virtual void draw(GamesEngineeringBase::Window& window) override {
        GERenderer::render_image(window, image, get_position());
    }
    const GamesEngineeringBase::Image& get_image() const {return image;}
    
    Vec2f get_centre() const {
        Vec2f size =  Vec2f(static_cast<float>(get_image().width), static_cast<float>(get_image().height));
        return get_position() + size;
    }
};

class WorldSpriteEntity : public SpriteEntity
{
protected:
    const Camera& camera;
public:
    WorldSpriteEntity(const Vec2f& _position, const std::string& image_filename, const Camera& _camera) : SpriteEntity(_position, image_filename), camera(_camera) {}
    
    virtual void draw(GamesEngineeringBase::Window& window) override {
        GERenderer::render_image(window, image, get_position() - camera.camera_offset);
    }
};

class CombatSprite : public WorldSpriteEntity
{
protected:
    float health;
    bool alive = true;
    
    Entity_Data entity_data_;
    Rect_Collider collider;
    
public:
    CombatSprite(
        const Vec2f& _position, 
        const Camera& _camera,
        Entity_Data entity_data) 
    : WorldSpriteEntity( _position, entity_data.sprite_img_filepath, _camera), 
        entity_data_(entity_data),
        health(entity_data.max_health),
        collider(get_image().width, get_image().height, _position, _camera) {}
    
    virtual void update(float delta_time) override {
        // Moving collider
        collider.set_position(get_position());
    }
    
    const Rect_Collider& get_collider() const {return collider;}
    bool is_alive() const {return alive;}
    
    void damage(const float damage) 
    {
        // Uses defense - 100 to take a percentage of damage
        // For example if defense is, the entity will take 75% of damage
        float applied_damage = ((100 - entity_data_.defense) / 100) * damage;
        health -= applied_damage;
        
        std::cout << "Health: " << health << "\n";
        if (health <= 0.0f) {std::cout << "You died\n";}
    }
    void heal(const float heal) {
        health += heal;
        if (health >= entity_data_.max_health) { health = entity_data_.max_health; }
        std::cout << "Health: " << health << "\n";
    }
    void apply_melee_damage(CombatSprite& other_entity, float delta_time) const {
        other_entity.damage(entity_data_.melee_damage * delta_time);
    }
};
