#pragma once

struct Vec2f
{
    float x;
    float y;
    Vec2f()
    {
        x = 0;
        y = 0;
    }
    Vec2f(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
    Vec2f operator+(const Vec2f n)
    {
        return Vec2f(x + n.x, y + n.y);
    }
    Vec2f operator*(const float scalar)
    {
        return Vec2f(x * scalar, y *scalar);
    }
    float dotProduct(const Vec2f n)
    {
        return (x * n.x) + (y * n.y);
    }
};
