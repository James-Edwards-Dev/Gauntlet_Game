#pragma once
#include "GameEntities.h"
#include "Players.h"

class Pick_Up_Item : public CollisionSprite
{
    const bool remove_on_pick_up;
    bool picked_up = false;
    
public:
    Pick_Up_Item(const Vec2f& _position, const std::string& image_filename, const Camera& _camera, bool _remove_on_pick_up = true);
    
    virtual void pick_up_effect(Player& hero) =0;
    
    virtual void draw(GamesEngineeringBase::Window& window) override;
    void on_hero_collide(Player& hero);
};

class Finish_Ladder : public Pick_Up_Item
{
public:
    Finish_Ladder(const Vec2f& _position, const Camera& _camera);
    
    virtual void pick_up_effect(Player& hero) override;
};
