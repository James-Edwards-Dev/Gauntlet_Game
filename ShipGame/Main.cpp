
#include "Entity.h"
#include "GameEntities.h"
#include "GamesEngineeringBase.h"
#include "Vectors.h"

int main()
{
    GamesEngineeringBase::Window window;
    window.create(1024, 720, "James's Ship Game");
    
    BackgroundEntity background(Vec2f(0, 0));
    
    World world;
    
    TileMap tile_map({0, 0}, world);
    HeroEntity hero({100, 100}, world);
    
    GamesEngineeringBase::Timer timer;
    float delta_time = timer.dt();
    
    bool running = true;
    while (running)
    {
        delta_time = timer.dt();
        
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
        
        // world.move_view({0.2f, 0});
        
        background.draw(window);
        
        tile_map.draw_tile(window, {-16.0f, -16.0f}, 0);
        tile_map.draw_tile(window, {1008.f, 704.0f}, 99);
        
        hero.draw(window);
        window.present();
    }
    
    return 0;
}
