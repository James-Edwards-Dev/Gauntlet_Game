#include "Pick_Up_Item.h"

Pick_Up_Item::Pick_Up_Item(const Vec2f& _position, const std::string& image_filename, const Camera& _camera, bool _remove_on_pick_up) 
    : CollisionSprite(_position, image_filename, _camera), remove_on_pick_up(_remove_on_pick_up)
{
    
}

void Pick_Up_Item::draw(GamesEngineeringBase::Window& window) {
    if (!(remove_on_pick_up && picked_up)) {
        CollisionSprite::draw(window);
    }
}

void Pick_Up_Item::on_hero_collide(HeroEntity& hero) {
    picked_up = true;
    pick_up_effect(hero);
}


Finish_Ladder::Finish_Ladder(const Vec2f& _position, const Camera& _camera) 
: Pick_Up_Item(_position, "resources/finish_ladder.png", _camera, false)
{
    
}

void Finish_Ladder::pick_up_effect(HeroEntity& hero) {
    hero.finish_line_reached = true;
    std::cout << "Finished" << std::endl;
}
