#pragma once

#include "GamesEngineeringBase.h"
#include "Vectors.h"

namespace GERenderer
{
    inline void render_rect_outline(GamesEngineeringBase::Window& window, float width, float height, Vec2f position) 
    {
        const int top = static_cast<int>(position.y);
        const int bottom = static_cast<int>(position.y + height);
        const int left = static_cast<int>(position.x); 
        const int right = static_cast<int>(position.x + width);
        
        // If rect is offscreen don't draw
        if (right < 0 || left >= static_cast<int>(window.getWidth())) return;
        if (bottom < 0 || top >= static_cast<int>(window.getHeight())) return;
        
        // Drawing top if top and bottom lines
        for (unsigned int col = 0; col <= width; col++) 
        {
            const int x = position.x + static_cast<int>(col);
            
            if (x < 0 || x >= window.getWidth()) continue;
            
            if (top >= 0) {
                window.draw(x, top, 255, 0, 0);
            }
            if (bottom < window.getHeight()) {
                window.draw(x, bottom, 255, 0, 0);
            }
        }
        // Drawing left and right lines
        // We are starting at 1 and checking if < instead of <= height so that corners are only drawn once
        for (unsigned int row = 1; row < height; row++) 
        {
            const int y = position.y + static_cast<int>(row);
            
            if (y < 0 || y >= window.getHeight()) continue;
            
            if (left >= 0) {
                window.draw(left, y, 255, 0, 0);
            }
            if (right < window.getWidth()) {
                window.draw(right, y, 255, 0, 0);
            }
        }
    }
    
    inline void render_image(GamesEngineeringBase::Window& window, const GamesEngineeringBase::Image& image, Vec2f position) 
    {
        for (unsigned int col = 0; col < image.width; col++) 
        {
            const int x = position.x + static_cast<int>(col);
            
            if (x < 0 || x >= window.getWidth()) continue;
            
            for (unsigned int row = 0; row < image.height; row++)
            {
                const int y = position.y + static_cast<int>(row);
                
                if (y < 0 || y >= window.getHeight()) continue;
                
                const unsigned int pixelIndex = (row * image.width + col) * image.channels;
                
                if (image.channels == 3 || image.data[pixelIndex + 3] != 0) {
                    window.draw(x, y, &image.data[pixelIndex]);
                }
            }
        }
    }
    inline void render_tile(GamesEngineeringBase::Window& window, const GamesEngineeringBase::Image& tile_map_image, const Vec2f& location, unsigned int tile_index, unsigned int size_x, unsigned int size_y, unsigned int tiles_in_row) 
    {
        unsigned int tile_x = tile_index % tiles_in_row;
        unsigned int tile_y = tile_index / tiles_in_row;
        
        unsigned int start_x = tile_x * size_x;
        unsigned int start_y = tile_y * size_y;
        
        for (unsigned int col = start_y; col < start_y + 32; col++) {
            
            unsigned int draw_y = static_cast<int>(location.y) + col - start_y;
            
            if (draw_y < 0 || draw_y >= window.getHeight()) continue;
            
            for (unsigned int row = start_x; row < start_x + 32; row++) {
                
                unsigned int draw_x = static_cast<int>(location.x) + row - start_x;
                
                if (draw_x < 0 || draw_x >= window.getWidth()) continue;
                
                const unsigned int pixelIndex = (col * tile_map_image.width + row) * tile_map_image.channels;
                
                if (tile_map_image.channels == 3 || tile_map_image.data[pixelIndex + 3] != 0) {
                    window.draw(draw_x, draw_y, &tile_map_image.data[pixelIndex]);
                }
            }
        }
    }
}
