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
    const float min_speed = 55.0f;
    const float max_speed = 125.0f;
    float speed = min_speed;
    
    unsigned int view_width;
    unsigned int view_height;
    Vec2f centre;
    
public:
    FollowCamera(unsigned int window_width, unsigned int window_height) : Camera(),  
    view_width(window_width),  view_height(window_height)
    {
        centre = Vec2f{static_cast<float>(view_width) / 2, static_cast<float>(view_height) / 2};
    }
    
private:
    // Calculate the desired camera location
    Vec2f calc_target_location(Vec2f alive_player_locations[], unsigned int alive_player_count) const {
        if (alive_player_count <= 0) {return camera_offset;}
        
        Vec2f target_location{0.f, 0.f};
        for (unsigned int i = 0; i < alive_player_count; ++i) {
            target_location =  target_location + alive_player_locations[i] + Vec2f{16.0f, 16.0f};
        }
        return  target_location / static_cast<float>(alive_player_count) - centre;
    }
    
public:
    // Moves camera to target location instantly
    void snap_to_target_location(Vec2f alive_player_locations[], unsigned int alive_player_count) {
        camera_offset = calc_target_location(alive_player_locations, alive_player_count);
    }
    // Moves camera towards target location
    void update(float delta_time, Vec2f alive_player_locations[], unsigned int alive_player_count) {
        Vec2f target_location = calc_target_location(alive_player_locations, alive_player_count);
        
        if (target_location == camera_offset) return; // This will be trigger if there are no alive players
        
        // Calculate distance from target squared
        float distanceSquared = camera_offset.distanceSquared(target_location);
        
        // Speed up when far away
        if (distanceSquared > 1500.0f) {
            speed = 150.0f;
        }
        else {
            speed = 55.0f;
        }
        
        // Moving camera
        if (distanceSquared > 0.1f) {
            Vec2f direction = (target_location - camera_offset).normalize();
            camera_offset = camera_offset + direction * delta_time * speed;
        }
    }
};