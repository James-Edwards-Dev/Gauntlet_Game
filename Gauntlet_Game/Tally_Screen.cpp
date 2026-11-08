#include "Tally_Screen.h"

Tally_Screen::Tally_Screen(GamesEngineeringBase::Window& window_, Gameplay_Stats* game_end_stats_, int num_of_characters_) 
: window(window_), game_end_stats(game_end_stats_), num_of_characters(num_of_characters_)
{
    text_image.load("resources/Valor8x8_CP437_transparent.png");
}

void Tally_Screen::open_menu(bool& running) {
    float restart_timer = 10;
    bool winning_game = false;
    
    unsigned int window_width = window.getWidth();
    unsigned int window_height = window.getHeight();
    
    unsigned int section_size = window_width / 3;
    
    BackgroundEntity background(Vec2f(0, 0));
    
    SpriteEntity* player_sprites[3];
    for (unsigned int i = 0; i < num_of_characters; i++) {
        // Detect if it was a winning game
        if (game_end_stats[i].victory) winning_game = true;
        // Create sprite
        player_sprites[i] = new SpriteEntity(
            Vec2f{static_cast<float>(section_size * i) + section_size / 2.f, window_height / 2.f - 96.f}, 
            get_sprite_image_filename(game_end_stats[i].hero_type));
    }
    
    GamesEngineeringBase::Timer timer;
    float delta_time = timer.dt();
    
    while (running && restart_timer > 0) {
        window.clear();
        
        delta_time = timer.dt();
        int fps = round(1 / delta_time);
        restart_timer -= delta_time;
        
        if (window.keyPressed(VK_ESCAPE)) running = false;
        if (window.keyPressed(VK_SPACE)) restart_timer = 0;
        
        background.draw(window);
        for (unsigned int i = 0; i < num_of_characters; i++) {
            game_end_stats[i].draw({
                static_cast<float>(section_size * i) + 32.f, window_height / 2.f - 32.f}, 
                window, 
                text_image);
            player_sprites[i]->draw(window);
        }
        
        if (winning_game) {
            GERenderer::render_string(window, text_image, "You Win!", {window_width / 2 - 32.0f, 64.f});
        } else {
            GERenderer::render_string(window, text_image, "You Lose", {window_width / 2 - 32.0f, 64.f});
        }
        std::string restart_message = "Restarting in " + std::to_string(static_cast<int>(restart_timer)) + " seconds.";
        GERenderer::render_string(window, text_image, restart_message, {window_width / 2.f - 96.f, window_height - 96.f});
        GERenderer::render_string(window, text_image, "Press space to skip", {window_width / 2.f - 76.f, window_height - 80.f});
        
        GERenderer::render_string(window, text_image, "FPS " + std::to_string(fps), {8.0f, 8.0f});
        
        window.present();
    }
    
    for (unsigned int i = 0; i < num_of_characters; i++) {
        delete player_sprites[i];
    }
}

std::string Tally_Screen::get_sprite_image_filename(unsigned int hero_type) const {
    GamesEngineeringBase::Image image;
    switch (hero_type) {
    case 0:
        return "resources/warrior.png";
    case 1:
        return "resources/rogue.png";
    default:    
        return "resources/wizard.png";
    }
}


