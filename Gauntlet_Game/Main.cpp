
#include "Collision.h"
#include "Collision_Manager.h"
#include "Enemy_Manager.h"
#include "GameEntities.h"
#include "TileMap.h"
#include "GamesEngineeringBase.h"
#include "MainMenu.h"
#include "Pick_Up_Item.h"
#include "Players.h"
#include "Projectile.h"
#include "Tally_Screen.h"
#include "Vectors.h"

Gameplay_Stats* start_game(GamesEngineeringBase::Window& window, bool& running, int characters[], unsigned int num_of_characters) {
    unsigned int window_width = window.getWidth();
    unsigned int window_height= window.getHeight();
    
    PlayerManager player_manager;
    EnemyManager enemy_manager;
    doubly_linked_list<Projectile*> projectile_manager;
    doubly_linked_list<Pick_Up_Item*> pick_up_manager;
    
    FollowCamera camera(window_width, window_height);
    
    // Creating heroes
    player_manager.create_players(characters, num_of_characters, camera, projectile_manager, enemy_manager);
    
    TileMap tile_map({0, 0}, camera);
    tile_map.load("resources/level.txt", enemy_manager, player_manager, projectile_manager, pick_up_manager);
    // tile_map.load("resources/test_arena.txt", enemy_manager, player_manager, projectile_manager, pick_up_manager);
    
    Collision_Manager collision_manager(player_manager, enemy_manager, projectile_manager, tile_map, pick_up_manager);
    
    GamesEngineeringBase::Image text_image;
    text_image.load("resources/Valor8x8_CP437_transparent.png");
    
    player_manager.reset_camera(camera);
    
    GamesEngineeringBase::Timer timer;
    float delta_time = timer.dt();
    
    bool playing = true;
    while (running && playing)
    {
        delta_time = timer.dt();
        
        int fps = round(1 / delta_time);
        
        window.checkInput();
        
        window.clear();
        
        if (window.keyPressed(VK_ESCAPE)) running = FALSE;
        
        tile_map.update(delta_time);
        
        enemy_manager.update(delta_time);
        for (doubly_node<Projectile*>* n = projectile_manager.head; n != nullptr; n = n->next) {
            n->data->update(delta_time);
        }
        
        collision_manager.manager_collision(delta_time);
        player_manager.update(delta_time, window, camera);

        tile_map.draw(window);
        for (doubly_node<Projectile*>* n = projectile_manager.head; n != nullptr; n = n->next) {
            n->data->draw(window);
            // n->data->draw_collider(window);
        }
        for (doubly_node<Pick_Up_Item*>* n = pick_up_manager.head; n != nullptr; n = n->next) {
            n->data->draw(window);
            // n->data->draw_collider(window);
        }
        player_manager.draw(window);
        enemy_manager.draw(window);
        
        playing = !player_manager.should_game_end();
        
        GERenderer::render_string(window, text_image, "FPS " + std::to_string(fps), {8.0f, 8.0f});
        
        window.present();
    }
    
    // Deleting all pick up's and projectiles
    doubly_node<Pick_Up_Item*>* pick_up_item_node = pick_up_manager.head;
    while (pick_up_item_node != nullptr) {
        doubly_node<Pick_Up_Item*>* next = pick_up_item_node->next;
        
        delete pick_up_item_node->data;
        
        pick_up_item_node = next;
    }
    doubly_node<Projectile*>* projectile_node = projectile_manager.head;
    while (projectile_node != nullptr) {
        doubly_node<Projectile*>* next = projectile_node->next;
        
        delete projectile_node->data;
        
        projectile_node = next;
    }
    
    return player_manager.get_game_end_stats();
}

int main() {
    bool running = true;
    
    unsigned int window_width = 640;
    unsigned int window_height= 480;

    GamesEngineeringBase::Window window;
    window.create(window_width, window_height, "Gauntlet_Game");
    
    while (running) {
        MainMenu* main_menu = new MainMenu(window);
        
        main_menu->open_menu(running);
    
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
        
        // Check if we have stopped running before standing game
        if (!running) return 0;
        
        Gameplay_Stats* game_end_stats = start_game(window, running, characters, num_of_characters);
        
        if (!running) {
            delete[] game_end_stats;
            return 0;
        }
        
        Tally_Screen* tally_screen = new Tally_Screen(window, game_end_stats, num_of_characters);
        tally_screen->open_menu(running);
        delete[] game_end_stats;
        
        if (!running) return 0;
    }
    
    return 0;
}
