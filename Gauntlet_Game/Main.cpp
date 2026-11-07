
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

void start_game(GamesEngineeringBase::Window& window) {
    unsigned int window_width = window.getWidth();
    unsigned int window_height= window.getHeight();
    
    PlayerManager player_manager;
    EnemyManager enemy_manager;
    doubly_linked_list<Projectile*> projectile_manager;
    
    FollowCamera camera(window_width, window_height);
    
    PlayerRogue hero1({32.0f, 64.0f}, camera, projectile_manager, 1);
    PlayerWizard hero2({64.0f, 64.0f}, camera, projectile_manager, 2);
    PlayerWarrior hero3({96.0f, 64.0f}, camera, projectile_manager, 3);
    player_manager.add(&hero1);
    // player_manager.add(&hero2);
    // player_manager.add(&hero3);
    
    TileMap tile_map({0, 0}, camera);
    tile_map.load("resources/test_map.txt", enemy_manager, player_manager, projectile_manager);
    // tile_map.load("resources/level_map.txt", enemy_manager, player_manager, projectile_manager);
    
    Collision_Manager collision_manager(player_manager, enemy_manager, projectile_manager, tile_map);
    
    GamesEngineeringBase::Image text_image;
    text_image.load("resources/Valor8x8_CP437_transparent.png");
    
    player_manager.reset_camera(camera);
    
    // EnemySpawner* goblin_spawner =  new EnemySpawner({320.f, 320.f}, camera, player_manager, enemy_manager, projectile_manager, Entity_Data(E_Goblin));
    // EnemySpawner* barbarian_spawner =  new EnemySpawner({480.f, 320.f}, camera, player_manager, enemy_manager, projectile_manager, Entity_Data(E_Barbarian));
    // enemy_manager.add(goblin_spawner);
    // enemy_manager.add(barbarian_spawner);
    
    GamesEngineeringBase::Timer timer;
    float delta_time = timer.dt();
    
    bool running = true;
    while (running)
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
        
        GERenderer::render_string(window, text_image, "FPS " + std::to_string(fps), {8.0f, 8.0f});
        
        window.present();
    }
}

int main() {
    unsigned int window_width = 640;
    unsigned int window_height= 480;

    GamesEngineeringBase::Window window;
    window.create(window_width, window_height, "Gauntlet_Game");

    MainMenu main_menu(window);

    main_menu.open_menu();
    if (!main_menu.get_running()) {return 0;}

    start_game(window);
}
