#pragma once
#include "GamesEngineeringBase.h"
#include "Players.h"

class Tally_Screen
{
    GamesEngineeringBase::Window& window;
    GamesEngineeringBase::Image text_image;
    Gameplay_Stats* game_end_stats;
    int num_of_characters;
    
public:
    
    Tally_Screen(GamesEngineeringBase::Window& window_, Gameplay_Stats* game_end_stats_, int num_of_characters_);
    
    void open_menu(bool& running);
    
private:
    std::string get_sprite_image_filename(unsigned int hero_type) const;
};
