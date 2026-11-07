
#include "Collision.h"
#include "Collision_Manager.h"
#include "Enemy_Manager.h"
#include "GameEntities.h"
#include "TileMap.h"
#include "GamesEngineeringBase.h"
#include "MainMenu.h"
#include "Players.h"
#include "Projectile.h"
#include "Vectors.h"

void open_main_menu(GamesEngineeringBase::Window& window);

void start_game(GamesEngineeringBase::Window& window, int characters[], unsigned int num_of_characters) {
    unsigned int window_width = window.getWidth();
    unsigned int window_height= window.getHeight();
    
    PlayerManager player_manager;
    EnemyManager enemy_manager;
    doubly_linked_list<Projectile*> projectile_manager;
    
    FollowCamera camera(window_width, window_height);
    
    // Creating heroes
    player_manager.create_players(characters, num_of_characters, camera, projectile_manager);
    
    TileMap tile_map({0, 0}, camera);
    tile_map.load("resources/test_map.txt", enemy_manager, player_manager, projectile_manager);
    // tile_map.load("resources/level_map.txt", enemy_manager, player_manager, projectile_manager);
    
    Collision_Manager collision_manager(player_manager, enemy_manager, projectile_manager, tile_map);
    
    GamesEngineeringBase::Image text_image;
    text_image.load("resources/Valor8x8_CP437_transparent.png");
    
    player_manager.reset_camera(camera);
    
    GamesEngineeringBase::Timer timer;
    float delta_time = timer.dt();
    
    bool running = true;
    bool playing = true;
    while (running && playing)
    {
        delta_time = timer.dt();
        
        int fps = round(1 / delta_time);
        
        window.checkInput();
        
        window.clear();
        
        if (window.keyPressed(VK_ESCAPE)) running = FALSE;
        
        tile_map.update(delta_time);
        
        player_manager.update(delta_time, window, camera);
        enemy_manager.update(delta_time);
        for (doubly_node<Projectile*>* n = projectile_manager.head; n != nullptr; n = n->next) {
            n->data->update(delta_time);
        }
        
        collision_manager.manager_collision(delta_time);

        tile_map.draw(window);
        for (doubly_node<Projectile*>* n = projectile_manager.head; n != nullptr; n = n->next) {
            n->data->draw(window);
            // n->data->draw_collider(window);
        }
        player_manager.draw(window);
        enemy_manager.draw(window);
        
        playing = (player_manager.get_alive_players() > 0);
        
        GERenderer::render_string(window, text_image, "FPS " + std::to_string(fps), {8.0f, 8.0f});
        
        window.present();
    }
    open_main_menu(window);
}

void open_main_menu(GamesEngineeringBase::Window& window) {
    MainMenu* main_menu = new MainMenu(window);

    main_menu->open_menu();
    if (!main_menu->get_running()) {return;}
    
    int characters[3];
    unsigned int num_of_characters = 0;
    if (main_menu->player_1_selector->has_joined()) {
        characters[num_of_characters] = main_menu->player_1_selector->get_character_type();
        num_of_characters++;
    }
    if (main_menu->player_2_selector->has_joined()) {
        characters[num_of_characters] = main_menu->player_2_selector->get_character_type();
        num_of_characters++;
    }
    if (main_menu->player_3_selector->has_joined()) {
        characters[num_of_characters] = main_menu->player_3_selector->get_character_type();
        num_of_characters++;
    }
    
    delete main_menu;
    
    start_game(window, characters, num_of_characters);
}

int main() {
    unsigned int window_width = 640;
    unsigned int window_height= 480;

    GamesEngineeringBase::Window window;
    window.create(window_width, window_height, "Gauntlet_Game");
    
    open_main_menu(window);
    
    return 0;
}
