#pragma once

#include "GamesEngineeringBase.h"
#include "Vectors.h"

class Entity
{
public:
    Entity() = default;
    
    virtual void update(float delta_time) =0;
};

class PositionEntity : public Entity
{
private:
    Vec2f position;
    
public:
    PositionEntity(const Vec2f& _position) : Entity(), position(_position) {}
    
    const Vec2f& get_position() const {
        return position;
    }
    void set_position(const Vec2f& _position) {
        position = _position;
    }
    void move(const Vec2f& delta) {
        position = position + delta;
    }
};

class VisualEntity : public PositionEntity
{
public:
    VisualEntity(const Vec2f& _position) : PositionEntity(_position) {}
    
    virtual void draw(GamesEngineeringBase::Window& window) =0;
};

class BackgroundEntity : public VisualEntity
{
public:
    BackgroundEntity(const Vec2f& _position) : VisualEntity(_position) {}
    
    virtual void update(float delta_time) override {
        // Background might not have any update logic for now
    }
    
    virtual void draw(GamesEngineeringBase::Window& window) override 
    {
        for (int y = 0; y < window.getHeight(); y += 1) {
            for (int x = 0; x < window.getWidth(); x += 1) {
                window.draw(x, y, 30, 20, 20);
            }
        }
    }
};