#pragma once

#include "Vectors.h"
#include "GameEntities.h"
#include "GERenderer.h"
#include <fstream>
#include <sstream>

class TileMap : public WorldSpriteEntity
{
private:
    unsigned int size_x = 32;
    unsigned int size_y = 32;
    unsigned int tiles_in_row;
    unsigned int tiles_in_col;
    unsigned int total_tiles;
    
    static constexpr int layout_col = 22;
    static constexpr int layout_row = 32;
    
    int* layout;
    
    unsigned int layout_old [layout_col][layout_row]
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

public:
    TileMap(const Vec2f& _position, const World& _world) : WorldSpriteEntity(_position, "resources/tileset.png", _world) 
    {
        tiles_in_row = image.width / size_x;
        tiles_in_col = image.height / size_y;
        total_tiles = tiles_in_row * tiles_in_col;
    }
    ~TileMap() {
        delete[] layout;
    }
    virtual void update(float delta_time) override {
        // TileMap will not do anything when updating for now
    }
    virtual void draw(GamesEngineeringBase::Window& window) override {
        int  layout_size = layout_col * layout_row;
        for (int i = 0; i < layout_size; i++) {
            int x = i / layout_col;
            int y = i % layout_col;
            
            if (layout_old[y][x] > total_tiles) { return; }
            
            Vec2f location;
            location.x = x * size_x;
            location.y = y * size_y;
            
            GERenderer::render_tile(window, get_image(), location - world.get_offset(), layout_old[y][x], size_x, size_y, tiles_in_row);
        }
    }
    
    void load(std::string level_filepath) 
    {
        std::ifstream infile(level_filepath);
        if (!infile.is_open()) {
            std::cout << "File " << level_filepath << "failed to open\n";
            return;
        }
        
        int width;
        int height;
        infile >> width >> height;
        
        layout = new int[width * height];
        
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
        for (int i = 0; i < width * height; i++) {
            std::cout << layout[i] << " ";
        }
    }
};