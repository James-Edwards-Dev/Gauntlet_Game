
#include "GamesEngineeringBase.h"

int main()
{
    GamesEngineeringBase::Window window;
    window.create(1024, 720, "James's Ship Game");
    
    bool running = true;
    while (running)
    {
        window.checkInput();
        
        window.clear();
        
        if (window.keyPressed(VK_ESCAPE)) running = FALSE;
        
        window.present();
    }
    
    return 0;
}
