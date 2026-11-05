
#include "Collision.h"
#include "Collision_Manager.h"
#include "Enemys.h"
#include "GameEntities.h"
#include "TileMap.h"
#include "GamesEngineeringBase.h"
#include "Players.h"
#include "Projectile.h"
#include "Vectors.h"

int main()
{
    unsigned int window_width = 640;
    unsigned int window_height= 480;
    
    GamesEngineeringBase::Window window;
    window.create(window_width, window_height, "Gauntlet_Game");
    
    PlayerManager player_manager;
    EnemyManager enemy_manager;
    doubly_linked_list<Projectile*> projectile_manager;
    
    FollowCamera camera(window_width, window_height);
    
    TileMap tile_map({0, 0}, camera);
    tile_map.load("resources/test_map.txt");
    // tile_map.load("resources/tileset_map.txt");
    
    Collision_Manager collision_manager(player_manager, enemy_manager, projectile_manager, tile_map);
    
    GamesEngineeringBase::Image text_image;
    text_image.load("resources/Valor8x8_CP437_transparent.png");
    
    // PlayerWarrior hero({96.0f, 64.0f}, camera, projectile_manager);
    PlayerRogue hero({96.0f, 64.0f}, camera, projectile_manager);
    // PlayerWizard hero({96.0f, 64.0f}, camera, projectile_manager);
    player_manager.add(&hero);
    
    player_manager.reset_camera(camera);
    
    Enemy enemy1({320.f, 320.f}, camera, player_manager, projectile_manager, Entity_Data(E_Goblin));
    Enemy enemy2({352.f, 320.f}, camera, player_manager, projectile_manager, Entity_Data(E_Goblin));
    enemy_manager.add(&enemy1);
    enemy_manager.add(&enemy2);
    
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
        
        Vec2f input{0, 0};
        if (window.keyPressed('W')) input.y = -1.0f;
        if (window.keyPressed('S')) input.y = 1.0f;
        if (window.keyPressed('A')) input.x = -1.0f;
        if (window.keyPressed('D')) input.x = 1.0f ;
        
        hero.set_input_axis(input);
        if (window.keyPressed('Q')) hero.fire_projectile();
        
        tile_map.update(delta_time);
        
        player_manager.update(delta_time, camera);
        
        hero.update(delta_time);
        enemy_manager.update(delta_time);
        for (doubly_node<Projectile*>* n = projectile_manager.head; n != nullptr; n = n->next) {
            n->data->update(delta_time);
        }
        
        collision_manager.manager_collision(delta_time);

        tile_map.draw(window);
        for (doubly_node<Projectile*>* n = projectile_manager.head; n != nullptr; n = n->next) {
            n->data->draw(window);
        }
        hero.draw(window);
        enemy_manager.draw(window);
        
        GERenderer::render_string(window, text_image, "FPS " + std::to_string(fps), {8.0f, 8.0f});
        
        window.present();
    }
    
    return 0;
}
