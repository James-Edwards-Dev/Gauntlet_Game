#pragma once
#include "Vectors.h"

class World
{
    Vec2f offset;
    Vec2f offset_change{0.0f, 0.0f};
    
public:
    const Vec2f& get_offset() const { return offset;}
    void set_offset(const Vec2f& _offset) {
        offset_change = offset - _offset;
        offset = _offset;
    }
    
    const Vec2f& get_offset_change() const { return offset_change;}
    
    void move_view(const Vec2f& delta) {
        set_offset(offset + delta);
    }
};
