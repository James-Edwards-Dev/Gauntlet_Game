#pragma once

#include "GamesEngineeringBase.h"
#include "GERenderer.h"
#include "Vectors.h"

class Collider 
{
    Vec2f position_;
    
public:
    Collider(const Vec2f& position) : position_(position) {}
    
    virtual void draw_debug_lines(GamesEngineeringBase::Window& window, unsigned char r = 255, unsigned char b = 0, unsigned char g = 0) const =0;
    
    Vec2f get_position() const {return position_;}
    void set_position(const Vec2f& position) {position_ = position;}
};

class Rect_Collider : public Collider
{
    float width_;
    float height_;
    
public:
    Rect_Collider(float width, float height, const Vec2f& position) : Collider(position), width_(width), height_(height) {}
    
    virtual void draw_debug_lines(GamesEngineeringBase::Window& window, unsigned char r = 255, unsigned char b = 0, unsigned char g = 0) const override {
        GERenderer::render_rect_outline(window, width_, height_, get_position(), r, g ,b );
    }
    
    float get_width() const{return width_;}
    float get_height() const{return height_;}
    
    bool is_overlapping(const Rect_Collider& otherRect) const {
        return (
            get_position().x + get_width() >= otherRect.get_position().x && 
            otherRect.get_position().x + otherRect.get_width() >= get_position().x 
            && get_position().y + get_height() >= otherRect.get_position().y &&
            otherRect.get_position().y + otherRect.get_height() >= get_position().y);
    }
};