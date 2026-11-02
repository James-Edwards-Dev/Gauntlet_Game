
#include "Collision.h"
#include "Entity.h"
#include "GameEntities.h"
#include "TileMap.h"
#include "GamesEngineeringBase.h"
#include "Vectors.h"

int main()
{
    unsigned int window_width = 1024;
    unsigned int window_height= 1024;
    
    GamesEngineeringBase::Window window;
    window.create(window_width, window_height, "Gauntlet_Game");
    
    BackgroundEntity background(Vec2f(0, 0));
    
    FollowCamera camera(window_width, window_height);
    
    TileMap tile_map({0, 0}, camera);
    tile_map.load("resources/test_map.txt");
    // tile_map.load("resources/tileset_map.txt");
    
    HeroEntity hero({64.0f, 64.0f}, camera);
    camera.add_target(&hero);
    
    GamesEngineeringBase::Timer timer;
    float delta_time = timer.dt();
    
    bool running = true;
    while (running)
    {
        delta_time = timer.dt();
        
        // std::cout << round(1 / delta_time) << "\n";
        
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
        
        tile_map.check_collision(hero, hero.get_collider());
        
        camera.update();
        
        tile_map.draw(window);
        // tile_map.draw_colliders(window);
        hero.draw(window);
        
        window.present();
    }
    
    return 0;
}
