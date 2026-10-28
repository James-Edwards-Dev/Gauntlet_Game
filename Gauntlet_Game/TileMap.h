#pragma once

#include "Vectors.h"
#include "GameEntities.h"
#include "GERenderer.h"
#include <fstream>
#include <sstream>

class TileMap : public WorldSpriteEntity
{
private:
    
    int width_in_tiles = 0;
    int height_in_tiles = 0;
    int size_in_titles = 0;
    
    int tile_width_pixels = 32;
    int tile_height_pixels = 32;
    
    const int tiles_in_sheet = 100;
    
    int* layout;

public:
    TileMap(const Vec2f& _position, const World& _world) : WorldSpriteEntity(_position, "resources/tileset.png", _world) {
        
    }
    virtual void update(float delta_time) override {
        // TileMap will not do anything when updating for now
    }
    virtual void draw(GamesEngineeringBase::Window& window) override {
        for (int i = 0; i < size_in_titles; i++) {
            int x = i % width_in_tiles;
            int y = i / width_in_tiles;
            
            if (layout[i] >= tiles_in_sheet) continue;
            
            Vec2f location;
            location.x = x * tile_width_pixels;
            location.y = y * tile_height_pixels;
            
            GERenderer::render_tile(window, get_image(), location - world.get_offset(), layout[i], tile_width_pixels, tile_height_pixels, width_in_tiles);
        }
    }
    
    void load(std::string level_filepath) 
    {
        std::ifstream infile(level_filepath);
        if (!infile.is_open()) {
            std::cout << "File " << level_filepath << "failed to open\n";
            return;
        }
        
        infile >> width_in_tiles >> height_in_tiles;
        size_in_titles = width_in_tiles * height_in_tiles;
        
        layout = new int[size_in_titles];
        
        std::string line;
        int next_array_index = 0;
        while (std::getline(infile, line)) {
            bool next_data_is_tile = true;
            int next_tile = 0;
            
            std::string word = line;
            std::stringstream stream(word);
            
            while (stream >> word) {
                if (next_data_is_tile) {
                    next_tile = std::stoi(word);
                }
                else {
                    for (int i = 0; i < std::stoi(word); i++) {
                        // std::cout << next_tile << " ";
                        layout[next_array_index] = next_tile;
                        next_array_index++;
                    }
                }
                next_data_is_tile = !next_data_is_tile;
            }
            // std::cout << "\n";
        }
        // for (int i = 0; i < size_in_titles; i++) {
        //     std::cout << layout[i] << " ";
        // }
    }
};