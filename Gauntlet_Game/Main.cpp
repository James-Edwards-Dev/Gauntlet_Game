
#include "Collision.h"
#include "Enemys.h"
#include "Entity.h"
#include "GameEntities.h"
#include "TileMap.h"
#include "GamesEngineeringBase.h"
#include "Players.h"
#include "Vectors.h"

int main()
{
    unsigned int window_width = 640;
    unsigned int window_height= 480;
    
    GamesEngineeringBase::Window window;
    window.create(window_width, window_height, "Gauntlet_Game");
    
    PlayerManager player_manager;
    EnemyManager enemy_manager;
    
    FollowCamera camera(window_width, window_height);
    
    TileMap tile_map({0, 0}, camera);
    tile_map.load("resources/test_map.txt");
    // tile_map.load("resources/tileset_map.txt");
    
    GamesEngineeringBase::Image text_image;
    text_image.load("resources/Valor8x8_CP437_transparent.png");
    
    HeroEntity hero({64.0f, 64.0f}, camera);
    player_manager.add(&hero);
    camera.add_target(&hero);
    camera.snap_to_target_location();
    
    Enemy enemy1({320.f, 320.f}, "resources/IdleAnimation2.png", camera, player_manager);
    Enemy enemy2({352.f, 320.f}, "resources/IdleAnimation2.png", camera, player_manager);
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
        
        tile_map.update(delta_time);
        
        hero.set_input_axis(input);
        hero.update(delta_time);
        enemy_manager.update(delta_time);
        
        tile_map.check_collision(hero, hero.get_collider());
        enemy_manager.check_collision(tile_map, player_manager, delta_time);
        
        camera.update(delta_time);
        
        tile_map.draw(window);
        // tile_map.draw_colliders(window);
        hero.draw(window);
        enemy_manager.draw(window);
        enemy_manager.draw_colliders(window);
        
        GERenderer::render_string(window, text_image, "FPS " + std::to_string(fps), {8.0f, 8.0f});
        
        window.present();
    }
    
    return 0;
}
