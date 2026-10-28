
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
    tile_map.load("resources/level_1.txt");
    
    HeroEntity hero({64.0f, 64.0f}, world);
    
    GamesEngineeringBase::Timer timer;
    float delta_time = timer.dt();
    
    bool running = true;
    while (running)
    {
        delta_time = timer.dt();
        
        std::cout << round(1 / delta_time) << "\n";
        
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
        
        tile_map.draw(window);
        
        hero.draw(window);
        window.present();
    }
    
    return 0;
}
