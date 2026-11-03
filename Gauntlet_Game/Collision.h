#pragma once

#include "GamesEngineeringBase.h"
#include "GERenderer.h"
#include "Vectors.h"
#include "Camera.h"

class Collider 
{
    Vec2f position_;
protected:
    const Camera& camera_;
    
public:
    Collider(const Vec2f& position, const Camera& _camera) : position_(position), camera_(_camera) {}
    
    virtual void draw_debug_lines(GamesEngineeringBase::Window& window, unsigned char r = 255, unsigned char b = 0, unsigned char g = 0) const =0;
    
    Vec2f get_position() const {return position_;}
    void set_position(const Vec2f& position) {position_ = position;}
};

class Rect_Collider : public Collider
{
    float width_;
    float height_;
    
public:
    Rect_Collider(float width, float height, const Vec2f& position, const Camera& camera) : Collider(position, camera), width_(width), height_(height) {}
    
    virtual void draw_debug_lines(GamesEngineeringBase::Window& window, unsigned char r = 255, unsigned char b = 0, unsigned char g = 0) const override {
        GERenderer::render_rect_outline(window, width_, height_, get_position() - camera_.camera_offset, r, g ,b );
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
private:
    Vec2f calculate_min_overlap_distance(const Rect_Collider& other_collider) const {
        // Calculate overlap distance
        float left_distance = other_collider.get_position().x + other_collider.get_width() - get_position().x ;
        float right_distance = get_position().x + get_width() - other_collider.get_position().x;
        float top_distance = other_collider.get_position().y + other_collider.get_height() - get_position().y;
        float bottom_distance = get_position().y + get_height() - other_collider.get_position().y;
                
        // Calculate min x and y overlap distance
        float distance_x;
        if (abs(left_distance) < abs(right_distance)) {
            distance_x = -left_distance;
        }
        else { 
            distance_x = right_distance;
        }
        float distance_y;
        if (abs(top_distance) < abs(bottom_distance)) {
            distance_y = -top_distance;
        }
        else {
            distance_y = bottom_distance;
        }
        // Calculate min overall distance and move player in that direction
        if (abs(distance_x) < abs(distance_y)) {
            return {distance_x, 0.f};
        }
        return {0.f, distance_y};
    }
public:
    void push_collide(PositionEntity& other_entity, const Rect_Collider& other_collider) const {
        other_entity.set_position(other_entity.get_position() + calculate_min_overlap_distance(other_collider));
    }
    void block_collide(PositionEntity& this_entity, PositionEntity& other_entity, const Rect_Collider& other_collider) const {
        this_entity.set_position(this_entity.get_position() - calculate_min_overlap_distance(other_collider) / 2);
        other_entity.set_position(other_entity.get_position() + calculate_min_overlap_distance(other_collider) / 2);
    }
};