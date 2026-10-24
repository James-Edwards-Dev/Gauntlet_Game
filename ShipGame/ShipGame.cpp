
#include "Entity.h"
#include "GamesEngineeringBase.h"
#include "Vectors.h"
#include "GERenderer.h"

int main()
{
    GamesEngineeringBase::Window window;
    window.create(1024, 720, "James's Ship Game");
    
    GamesEngineeringBase::Image image;
    image.load("resources/IdleAnimation2.png");
    
    BackgroundEntity background(Vec2f(0, 0));
    
    bool running = true;
    while (running)
    {
        window.checkInput();
        
        window.clear();
        
        background.draw(window);
        GERenderer::drawImage(window, image, Vec2f(0, 0));
        
        if (window.keyPressed(VK_ESCAPE)) running = FALSE;
        
        window.present();
    }
    
    return 0;
}
