#pragma once
#include "Vectors.h"

class World
{
    Vec2f offset;
    
public:
    const Vec2f& get_offset() const { return offset;}
    void set_offset(const Vec2f& _offset) {offset = _offset;}
    
    void move_view(const Vec2f& delta) {
        offset = offset + delta;
    }
};
