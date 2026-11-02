#pragma once

#include "Vectors.h"

class Camera
{
public:
    Vec2f camera_offset;
    
    void move(const float& delta_time, const Vec2f& distance) {
        camera_offset = camera_offset + distance * delta_time;
    }
};
