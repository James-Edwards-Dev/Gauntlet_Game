#include "MainMenu.h"

#include <iostream>

#include "GameEntities.h"

Character_Selector::Character_Selector(unsigned int player_number_, GamesEngineeringBase::Window& window_, GamesEngineeringBase::Image& text_image_)
: player_number(player_number_), window(window_), text_image(text_image_) {
    sprite_location = {
        static_cast<float>(window.getWidth() / 2 - 11), 
        static_cast<float>(window.getHeight() / 2 - 16)
    };
    switch (player_number) {
    case 1:
        right_key = 'A';
        left_key = 'D';
        ready_key = 'E';
        leave_key = 'Q';
        joined = true;
        sprite_location = sprite_location - Vec2f{150.0f, 0};
        break;
    case 2:
        right_key = 'H';
        left_key = 'F';
        ready_key = 'Y';
        leave_key = 'R';
        character_type = 1;
        break;
    default:
        right_key = VK_RIGHT;
        left_key = VK_LEFT;
        ready_key = 'M';
        leave_key = 'N';
        sprite_location = sprite_location + Vec2f{150.0f, 0};
        character_type = 2;
        break;
    }
    
    sprite[0] = new SpriteEntity(sprite_location, "resources/warrior.png");
    sprite[1] = new SpriteEntity(sprite_location, "resources/rogue.png");
    sprite[2] = new SpriteEntity(sprite_location, "resources/wizard.png");
}

Character_Selector::~Character_Selector() {
    for (unsigned int i = 0; i < 2; i++) {
        delete sprite[i];
    }
}

bool Character_Selector::has_joined() const {
    return joined;
}

bool Character_Selector::is_ready() const {
    return ready;
}

void Character_Selector::draw() const {
    if (joined) {
        sprite[character_type]->draw(window);
        GERenderer::render_string(window, text_image, get_character_name(), sprite[character_type]->get_position() + Vec2f{-10.f, 35.f});
        if (player_number != 3) {
            GERenderer::render_char(window, text_image, right_key, sprite[character_type]->get_centre() + Vec2f{25.0f, 0});
            GERenderer::render_char(window, text_image, left_key, sprite[character_type]->get_centre() + Vec2f{-25.0f, 0});
        }
        else {
            GERenderer::render_string(window, text_image, "->", sprite[character_type]->get_centre() + Vec2f{20.0f, 0});
            GERenderer::render_string(window, text_image, "<-", sprite[character_type]->get_centre() + Vec2f{-30.0f, 0});
        }
        
        // Drawing ready text
        std::string is_ready_string;
        if (ready) { is_ready_string = "yes"; } else {is_ready_string = "no";}
        std::string ready_string = "Ready (";
        ready_string += ready_key;
        ready_string += ") " + is_ready_string;
        GERenderer::render_string(window, text_image, ready_string, sprite[character_type]->get_centre() + Vec2f{-50.f, -35.f});
        
        // Drawing leave text
        std::string leave_string = "Press ";
        leave_string += leave_key;
        leave_string += " to leave";
        GERenderer::render_string(window, text_image, leave_string, sprite[character_type]->get_centre() + Vec2f{-55.f, 50});
    } else {
        std::string join_string = "Press ";
        join_string += ready_key;
        join_string += " to join!";
        GERenderer::render_string(window, text_image, join_string, sprite[character_type]->get_centre() + Vec2f{-50.f, 0});
    }
}

void Character_Selector::detect_input() {
    if (joined) {
        if (window.keyPressed(right_key)) {
            if (!right_key_held) {
                if (character_type == 0) {
                    character_type = 2;
                } else {
                    character_type--;
                }
                right_key_held = true;
            }
        } else {right_key_held = false;}
        if (window.keyPressed(left_key)) {
            if (!left_key_held) {
                if (character_type == 2) {
                    character_type = 0;
                } else {
                    character_type++;
                }
                left_key_held = true;
            }
        } else {left_key_held = false;}
    }
    if (window.keyPressed(leave_key)) {
        if (!leave_key_held) {
            joined = !joined;
            if (joined == false) {
                ready = false;
            }
            leave_key_held = true;
        }
    } else {leave_key_held = false;}
    if (window.keyPressed(ready_key)) {
        if (!ready_key_held) {
            if (joined == false) {
                joined = true;
            } else {
                ready = !ready;
            }
            ready_key_held = true;
        }
    } else {ready_key_held = false;}
}

std::string Character_Selector::get_character_name() const {
    switch (character_type) {
    case 0:
        return "Warrior";
    case 1:
        return "Rogue";
    default:
        return "Wizard";
    }
}

int Character_Selector::get_character_type() const {
    return character_type;
}

MainMenu::MainMenu(GamesEngineeringBase::Window& window_) : window(window_) {
    text_image.load("resources/Valor8x8_CP437_transparent.png");
    
    player_1_selector = new Character_Selector(1, window, text_image);
    player_2_selector = new Character_Selector(2, window, text_image);
    player_3_selector = new Character_Selector(3, window, text_image);
}

MainMenu::~MainMenu() {
    delete player_1_selector;
    delete player_2_selector;
    delete player_3_selector;
}

void MainMenu::open_menu(bool& running) {
    
    BackgroundEntity background(Vec2f(0, 0));
    
    GamesEngineeringBase::Timer timer;
    float delta_time = timer.dt();
    
    while (running && open) {
        window.clear();
        
        delta_time = timer.dt();
        int fps = round(1 / delta_time);
        
        if (window.keyPressed(VK_ESCAPE)) running = false;
        
        player_1_selector->detect_input();
        player_2_selector->detect_input();
        player_3_selector->detect_input();
        
        background.draw(window);
        player_1_selector->draw();
        player_2_selector->draw();
        player_3_selector->draw();
        
        GERenderer::render_string(window, text_image, "FPS " + std::to_string(fps), {8.0f, 8.0f});
        GERenderer::render_string(window, text_image, "Gauntlet Game", {8.0f, 32.0f});
        
        player_joined = player_1_selector->has_joined() || player_2_selector->has_joined() || player_3_selector->has_joined(); 
        all_ready = (!player_1_selector->has_joined() || player_1_selector->is_ready()) 
                 && (!player_2_selector->has_joined() || player_2_selector->is_ready())
                 && (!player_3_selector->has_joined() || player_3_selector->is_ready());
        open = !(player_joined && all_ready);
        
        window.present();
    }
}
