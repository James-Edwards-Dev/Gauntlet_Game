#pragma once

#include "Vectors.h"
#include "GameEntities.h"
#include "GERenderer.h"
#include "Data_Structs.h"

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
    const int tiles_per_row = 10;
    
    int* layout;
    
    // Stack containing the colliders of all impassable tiles
    Dynamic_Array<Rect_Collider*> colliders;

public:
    TileMap(const Vec2f& _position, const Camera& _camera) : WorldSpriteEntity(_position, "resources/tileset.png", _camera) {
        
    }
    ~TileMap() {
        delete[] layout;
        for (unsigned int i = 0; i < colliders.size(); i++) {
            delete colliders[i];
        }
    }
    virtual void draw(GamesEngineeringBase::Window& window) override {
        for (int i = 0; i < size_in_titles; i++) {
            int x = i % width_in_tiles;
            int y = i / width_in_tiles;
            
            if (layout[i] >= tiles_in_sheet) continue;
            
            Vec2f location;
            location.x = x * tile_width_pixels;
            location.y = y * tile_height_pixels;
            
            GERenderer::render_tile(window, get_image(), location - camera.camera_offset, layout[i], tile_width_pixels, tile_height_pixels, tiles_per_row);
        }
    }
    virtual void update(float delta_time) override {
        // Doesn't do anything 
    }
    void draw_colliders(GamesEngineeringBase::Window& window, unsigned char r=255, unsigned char g=0, unsigned char b=0) {
        for (unsigned int i = 0; i < colliders.size(); i++) {
            colliders[i]->draw_debug_lines(window);
        }
    }
    void block(SpriteEntity& entity, const Rect_Collider& collider) {
        for (unsigned int i = 0; i < colliders.size(); i++) {
            if (colliders[i]->is_overlapping(collider)) {
                colliders[i]->block_collide(entity, collider);
            }
        }
    }
    bool is_overlapping(const Rect_Collider& collider) {
        for (unsigned int i = 0; i < colliders.size(); i++) {
            if (colliders[i]->is_overlapping(collider)) {
                return true;
            }
        }
        return false;
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
                        layout[next_array_index] = next_tile;
                        // Creating Rects of Collision
                        // TODO change this to use Enums instead of the hardcoded number 13
                        if (next_tile == 13) {
                            float collider_x = (next_array_index % width_in_tiles) * tile_width_pixels;
                            float collider_y = (next_array_index / width_in_tiles) * tile_height_pixels;
                            
                            colliders.append(new Rect_Collider(tile_width_pixels, tile_height_pixels, Vec2f(collider_x, collider_y), camera));
                        }
                        
                        next_array_index++;
                    }
                }
                next_data_is_tile = !next_data_is_tile;
            }
        }
        infile.close();
    }
};
