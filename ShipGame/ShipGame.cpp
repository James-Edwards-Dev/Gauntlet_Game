
#include "GamesEngineeringBase.h"
#include "Vectors.h"
#include "GERenderer.h"

int main()
{
    GamesEngineeringBase::Window window;
    window.create(1024, 720, "James's Ship Game");
    
    GamesEngineeringBase::Image image;
    image.load("resources/L.png");
    
    bool running = true;
    while (running)
    {
        window.checkInput();
        
        window.clear();
        
        for (int x = 0; x < window.getWidth(); x ++) {
            for (int y = 0; y < window.getHeight(); y++) {
                window.draw(x, y, 255, 255, 0);
            }
        }
        
        GERenderer::drawImage(window, image, Vec2f(200.f, 650.0f));
        
        if (window.keyPressed(VK_ESCAPE)) running = FALSE;
        
        window.present();
    }
    
    return 0;
}
