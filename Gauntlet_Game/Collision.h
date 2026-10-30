#pragma once

#include "GamesEngineeringBase.h"
#include "Vectors.h"

class Collider 
{
    Vec2f position_;
    
public:
    Collider(const Vec2f& position) : position_(position) {}
    
    virtual void draw_debug_lines(GamesEngineeringBase::Window& window) =0;
    
    Vec2f get_position() const {return position_;}
    void set_position(const Vec2f& position) {position_ = position;}
};

class Rect_Collider : public Collider
{
    float width_;
    float height_;
    
public:
    Rect_Collider(float width, float height, const Vec2f& position) : Collider(position), width_(width), height_(height) {}
    
    virtual void draw_debug_lines(GamesEngineeringBase::Window& window) override {
    }
};