#pragma once
#include "Entity.h"
#include <iostream>

#include "GERenderer.h"
#include "Camera.h"

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
    
    virtual void update(float delta_time) override {
        // Doesn't do anything for now
    }
};

class CombatSprite : public WorldSpriteEntity
{
protected:
    float max_health;
    float health;
    float defence;
    float speed;
    
    Rect_Collider collider;
public:
    CombatSprite(const Vec2f& _position, const std::string& image_filename, const Camera& _camera, const float _max_health = 100, const float _speed = 100, const float _defense = 0) 
    : WorldSpriteEntity(_position, image_filename, _camera), max_health(_max_health), health(_max_health), speed(_speed), defence(_defense), 
    collider(get_image().width, get_image().height, _position, _camera) {}
    
    virtual void update(float delta_time) override {
        WorldSpriteEntity::update(delta_time);
        // Moving collider
        collider.set_position(get_position());
    }
    
    const Rect_Collider& get_collider() const {return collider;}
    
    void damage(const float damage) 
    {
        // Uses defense - 100 to take a percentage of damage
        // For example if defense is, the entity will take 75% of damage
        float applied_damage = ((100 - defence) / 100) * damage;
        health -= applied_damage;
        
        std::cout << "Health: " << health << "\n";
        if (health <= 0.0f) {std::cout << "You died\n";}
    }
    void heal(const float heal) {
        health += heal;
        if (health >= max_health) { health = max_health; }
        std::cout << "Health: " << health << "\n";
    }
};

class HeroEntity : public CombatSprite
{
public:
    HeroEntity(const Vec2f& _position, const Camera& _camera, const float _max_health = 100, const float _speed = 100, const float _defense = 0) 
    : CombatSprite(_position, "resources/IdleAnimation2.png", _camera, _max_health, _speed, _defense) {}
    
    void set_input_axis(const Vec2f& input) {inputAxis = input;}
    const Vec2f& get_input_axis() const {return inputAxis;}
    
    virtual void update(float delta_time) override {
        move(get_input_axis() * speed * delta_time);
        
        CombatSprite::update(delta_time);
    }
    
protected:
    Vec2f inputAxis{0.0f, 0.0f};
};
