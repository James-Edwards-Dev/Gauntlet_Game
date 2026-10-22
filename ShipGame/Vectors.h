#pragma once

struct Vec2f
{
    float x;
    float y;
    
    Vec2f(): x(0), y(0) {}
    Vec2f(float _x, float _y): x(_x), y(_y) {}
    
    Vec2f operator+(const Vec2f n) const
    {
        return Vec2f(x + n.x, y + n.y);
    }
    Vec2f operator*(const float scalar) const
    {
        return Vec2f(x * scalar, y *scalar);
    }
    float dot_product(const Vec2f n) const
    {
        return (x * n.x) + (y * n.y);
    }
};
