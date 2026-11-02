#pragma once

#include <cmath>

struct Vec2f
{
    float x;
    float y;
    
    Vec2f(): x(0), y(0) {}
    Vec2f(float _x, float _y): x(_x), y(_y) {}
    
    Vec2f operator+(const Vec2f n) const {
        return Vec2f{x + n.x, y + n.y};
    }
    Vec2f operator-(const Vec2f n) const {
        return Vec2f{x - n.x, y - n.y};
    }
    Vec2f operator*(const float scalar) const{
        return Vec2f{x * scalar, y *scalar};
    }
    float length() const {
        return std::sqrt(x * x + y * y);
    }
    float dot_product(const Vec2f n) const {
        return (x * n.x) + (y * n.y);
    }
    float distanceSquared(const Vec2f n) const {
        Vec2f difference = Vec2f{x, y} - n;
        return difference.dot_product(difference);
    }
    Vec2f normalize() const {
        float length = this->length();
        
        if (length > 0.0f) {
            return {x / length, y / length};
        }
        return {x, y};
    }
};
