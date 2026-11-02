#pragma once

#include "GameEntities.h"

class Enemy : public CombatSprite
{
public:
    Enemy(const Vec2f& _position, const std::string& image_filename, const Camera& _camera, const float _max_health = 100, const float _speed = 100, const float _defense = 0) 
    : CombatSprite(_position, image_filename, _camera, _max_health, _speed, _defense)
    {}
};
