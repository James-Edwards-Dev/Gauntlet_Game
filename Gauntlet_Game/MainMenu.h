#pragma once
#include "GameEntities.h"
#include "GamesEngineeringBase.h"

class MainMenu
{
    bool running = true;
    bool open = true;
    bool player_joined = true;
    bool all_ready = false;
    
    GamesEngineeringBase::Window& window;
public:
    MainMenu(GamesEngineeringBase::Window& window_);
    
    bool get_running() const {return running;}
    
    void open_menu();
};

class Character_Selector
{
    int character = 0;
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
};
