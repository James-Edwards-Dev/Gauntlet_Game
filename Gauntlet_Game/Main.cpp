
#include "Collision.h"
#include "Entity.h"
#include "GameEntities.h"
#include "TileMap.h"
#include "GamesEngineeringBase.h"
#include "Vectors.h"

int main()
{
    GamesEngineeringBase::Window window;
    window.create(1024, 704, "James's Ship Game");
    
    BackgroundEntity background(Vec2f(0, 0));
    
    World world;
    
    TileMap tile_map({0, 0}, world);
    tile_map.load("resources/test_map.txt");
    // tile_map.load("resources/tileset_map.txt");
    
    HeroEntity hero({64.0f, 64.0f}, world);
    Rect_Collider test_rect(32.0f, 32.0f, {128.0f, 128.0f}, world);
    
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
        
        hero.set_input_axis(input);
        hero.update(delta_time);
        
        test_rect.update();
        
        world.move_view({0.2f, 0});
        
        background.draw(window);
        
        tile_map.draw(window);
        
        hero.draw(window);
        
        unsigned char r = 0;
        unsigned char g = 255;
        if (hero.get_collider().is_overlapping(test_rect)) {
            r = 255;
            g = 0;
        }
        hero.get_collider().draw_debug_lines(window, r, 0, g);
        test_rect.draw_debug_lines(window, r, 0, g);
        
        window.present();
    }
    
    return 0;
}
