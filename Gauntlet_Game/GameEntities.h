#pragma once
#include "Entity.h"
#include <iostream>

#include "GERenderer.h"
#include "World.h"

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
    const World& world;
public:
    WorldSpriteEntity(const Vec2f& _position, const std::string& image_filename, const World& _world) : SpriteEntity(_position, image_filename), world(_world) {}
    
    virtual void update(float delta_time) override {
        // Doesn't do anything for now
    }
};

class HeroEntity : public WorldSpriteEntity
{
public:
    HeroEntity(const Vec2f& _position, const World& _world) 
    : WorldSpriteEntity(_position, "resources/IdleAnimation2.png", _world)
    , collider(get_image().width, get_image().height, _position, _world){}
    
    void set_input_axis(const Vec2f& input) {inputAxis = input;}
    const Vec2f& get_input_axis() const {return inputAxis;}
    
    const Rect_Collider& get_collider() const {return collider;}
    
    virtual void update(float delta_time) override {
        WorldSpriteEntity::update(delta_time);
        
        move(get_input_axis() * speed * delta_time);
        // Moving collider
        collider.set_position(get_position());
    }
    
protected:
    float speed = 100.0f;
    Vec2f inputAxis{0.0f, 0.0f};
    
    Rect_Collider collider;
};
