#pragma once
#include "GameEntities.h"
#include "GamesEngineeringBase.h"

class Character_Selector
{
    int character_type = 0;
    unsigned int player_number;
    
    int right_key;
    int left_key;
    int ready_key;
    int leave_key;
    
    bool right_key_held = false;
    bool left_key_held = false;
    
    bool joined = false;
    bool ready = false;
    bool leave_key_held = false;
    bool ready_key_held = false;
    
    Vec2f sprite_location;
    SpriteEntity* sprite[3];
    
    GamesEngineeringBase::Window& window;
    GamesEngineeringBase::Image& text_image;
public:
    Character_Selector(unsigned int player_number_, GamesEngineeringBase::Window& window_, GamesEngineeringBase::Image& text_image_);
    ~Character_Selector();
    
    bool has_joined() const;
    bool is_ready() const;
    
    void draw() const;
    
    void detect_input();
    
    std::string get_character_name() const;
    
    int get_character_type() const;
};

class MainMenu
{
    bool open = true;
    bool player_joined = true;
    bool all_ready = false;
    
    GamesEngineeringBase::Image text_image;
    
    GamesEngineeringBase::Window& window;
public:
    
    Character_Selector* player_1_selector;
    Character_Selector* player_2_selector;
    Character_Selector* player_3_selector;
    
    MainMenu(GamesEngineeringBase::Window& window_);
    ~MainMenu();
    
    void open_menu(bool& running);
};
