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
        GERenderer::drawImage(window, image, get_position());
    }
    const GamesEngineeringBase::Image& get_image() const {return image;}
};

// class WorldEntity : public SpriteEntity
// {
// private:
//     World& world;
// public:
//     WorldEntity(const Vec2f& _position, const std::string& image_filename, World& _world) : SpriteEntity(_position, image_filename), world(_world) {}
// };

class HeroEntity : public SpriteEntity
{
public:
    HeroEntity(const Vec2f& _position) : SpriteEntity(_position, "resources/IdleAnimation2.png") {}
    
    void set_input_axis(const Vec2f& input) {inputAxis = input;}
    const Vec2f& get_input_axis() const {return inputAxis;}
    
    virtual void update(float delta_time) override {
        move(get_input_axis() * speed * delta_time);
    }
    
protected:
    float speed = 64.0f;
    Vec2f inputAxis{0.0f, 0.0f};
    
};