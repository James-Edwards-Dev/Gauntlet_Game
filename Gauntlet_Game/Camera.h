#pragma once

#include "Entity.h"
#include "Vectors.h"

class Camera
{
public:
    Camera() = default;
    
    Vec2f camera_offset;
    
    void move(const float& delta_time, const Vec2f& distance) {
        camera_offset = camera_offset + distance * delta_time;
    }
};

class FollowCamera : public Camera
{
    float speed = 50.0f;
    
    int num_of_targets = 0;
    static constexpr int max_num_of_targets = 4;
    const PositionEntity* Targets[max_num_of_targets];
    
    unsigned int view_width;
    unsigned int view_height;
    Vec2f centre;
    
public:
    FollowCamera(unsigned int window_width, unsigned int window_height) : Camera(),  
    view_width(window_width),  view_height(window_height)
    {
        centre = Vec2f{static_cast<float>(view_width) / 2, static_cast<float>(view_height) / 2};
        
        for (unsigned int i = 0; i < max_num_of_targets; i++) {
            Targets[i] = nullptr;
        }
    }
    // Adds a target to the array of targets used by the camera
    // Returns False if array is full
    bool add_target(const PositionEntity* target) {
        if (num_of_targets >= max_num_of_targets) { return false; }
        
        Targets[num_of_targets] = target;
        num_of_targets++;
        
        return true;
    }
    
private:
    // Calculate the desired camera location
    Vec2f calc_target_location() const {
        if (num_of_targets >= max_num_of_targets) {return camera_offset;}
        
        return  Targets[0]->get_position() + Vec2f{16.0f, 16.0f} - centre;
    }
    
public:
    // Moves camera to target location instantly
    void snap_to_target_location() {
        camera_offset = calc_target_location();
    }
    // Moves camera towards target location
    void update(float delta_time) {
        // Calculate distance from target squared
        float distanceSquared = camera_offset.distanceSquared(calc_target_location());
        
        // Moving camera
        if (distanceSquared > 0.1f) {
            Vec2f direction = (calc_target_location() - camera_offset).normalize();
            camera_offset = camera_offset + direction * delta_time * speed;
        }
    }
};