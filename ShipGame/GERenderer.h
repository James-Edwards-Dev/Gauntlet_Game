#pragma once

#include "GamesEngineeringBase.h"
#include "Vectors.h"

namespace GERenderer
{
    inline void draw_pixel(
        GamesEngineeringBase::Window& window, 
        const GamesEngineeringBase::Image& image, 
        Vec2f position, 
        unsigned int pixel_x, 
        unsigned int pixel_y) 
    {
        const int x = position.x + static_cast<int>(pixel_y);
        if (x < 0 || x >= window.getWidth()) return;
        
        const int y = position.y + static_cast<int>(pixel_x);
        if (y < 0 || y >= window.getHeight()) return;
        
        const unsigned int pixelIndex = (pixel_x * image.width + pixel_y) * image.channels;
                
        if (image.channels == 3 || image.data[pixelIndex + 3] != 0) {
            window.draw(x, y, &image.data[pixelIndex]);
        }
    }
    
    inline void draw_image(GamesEngineeringBase::Window& window, const GamesEngineeringBase::Image& image, Vec2f position) 
    {
        for (unsigned int col = 0; col < image.width; col++) 
        {
            const int x = position.x + static_cast<int>(col);
            
            if (x < 0 || x >= window.getWidth()) continue;
            
            for (unsigned int row = 0; row < image.height; row++)
            {
                const int y = position.y + static_cast<int>(row);
                
                if (y < 0 || y >= window.getHeight()) continue;
                
                // const unsigned int pixelIndex = (row * image.width + col) * image.channels;
                //
                // if (image.channels == 3 || image.data[pixelIndex + 3] != 0) {
                //     window.draw(x, y, &image.data[pixelIndex]);
                // }
                
                draw_pixel(window, image, position, row, col);
            }
        }
    }
}
