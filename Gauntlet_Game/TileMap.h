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
    Stack<Rect_Collider*> collider_stack;

public:
    TileMap(const Vec2f& _position, const World& _world) : WorldSpriteEntity(_position, "resources/tileset.png", _world) {
        
    }
    ~TileMap() {
        delete[] layout;
        clear_stack(collider_stack);
    }
    virtual void update(float delta_time) override {
        for (node<Rect_Collider*>* n = collider_stack.top; n != nullptr; n = n->next) {
            n->data->update();
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
            
            GERenderer::render_tile(window, get_image(), location - world.get_offset(), layout[i], tile_width_pixels, tile_height_pixels, tiles_per_row);
        }
    }
    void draw_colliders(GamesEngineeringBase::Window& window, unsigned char r=255, unsigned char g=0, unsigned char b=0) {
        for (node<Rect_Collider*>* n = collider_stack.top; n != nullptr; n = n->next) {
            n->data->draw_debug_lines(window);
        }
    }
    void check_collision(SpriteEntity& entity, const Rect_Collider& collider) {
        for (node<Rect_Collider*>* n = collider_stack.top; n != nullptr; n = n->next) {
            if (n->data->is_overlapping(collider)) {
                // Calculate overlap distance
                float left_distance = entity.get_position().x + entity.get_image().width - n->data->get_position().x ;
                float right_distance = n->data->get_position().x + n->data->get_width() - entity.get_position().x;
                float top_distance = entity.get_position().y + entity.get_image().height - n->data->get_position().y;
                float bottom_distance = n->data->get_position().y + n->data->get_height() - entity.get_position().y;
                
                // Calculate min x and y overlap distance
                float distance_x;
                if (abs(left_distance) < abs(right_distance)) {
                    distance_x = -left_distance;
                }
                else { 
                    distance_x = right_distance;
                }
                float distance_y;
                if (abs(top_distance) < abs(bottom_distance)) {
                    distance_y = -top_distance;
                }
                else {
                    distance_y = bottom_distance;
                }
                // Calculate min overall distance and move player in that direction
                if (abs(distance_x) < abs(distance_y)) {
                    entity.set_position(entity.get_position() + Vec2f(distance_x, 0));
                }
                else {
                    entity.set_position(entity.get_position() + Vec2f(0, distance_y));
                }
            }
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
                        layout[next_array_index] = next_tile;
                        // Creating Rects of Collision
                        // TODO change this to use Enums instead of the hardcoded number 13
                        if (next_tile == 13) {
                            float collider_x = (next_array_index % width_in_tiles) * tile_width_pixels;
                            float collider_y = (next_array_index / width_in_tiles) * tile_height_pixels;
                            
                            collider_stack.push(new Rect_Collider(tile_width_pixels, tile_height_pixels, Vec2f(collider_x, collider_y), world));
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
