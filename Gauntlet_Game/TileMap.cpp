#include "TileMap.h"

#include <fstream>
#include <sstream>

#include "Pick_Up_Item.h"

TileMap::TileMap(const Vec2f& _position, const Camera& _camera) 
: WorldSpriteEntity(_position, "resources/tileset.png", _camera)
{
    
}

TileMap::~TileMap() {
    delete[] layout;
    for (unsigned int i = 0; i < colliders.size(); i++) {
        delete colliders[i];
    }
}

void TileMap::draw(GamesEngineeringBase::Window& window) {
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

void TileMap::update(float delta_time) {
    // Doesn't do anything 
}

void TileMap::draw_colliders(GamesEngineeringBase::Window& window, unsigned char r, unsigned char g, unsigned char b) {
    for (unsigned int i = 0; i < colliders.size(); i++) {
        colliders[i]->draw_debug_lines(window);
    }
}

void TileMap::block(SpriteEntity& entity, const Rect_Collider& collider) {
    for (unsigned int i = 0; i < colliders.size(); i++) {
        if (colliders[i]->is_overlapping(collider)) {
            colliders[i]->block_collide(entity, collider);
        }
    }
}

bool TileMap::is_overlapping(const Rect_Collider& collider) {
    for (unsigned int i = 0; i < colliders.size(); i++) {
        if (colliders[i]->is_overlapping(collider)) {
            return true;
        }
    }
    return false;
}

void TileMap::load(
    std::string level_filepath, 
    EnemyManager& enemy_manager, 
    const PlayerManager& player_manager, 
    doubly_linked_list<Projectile*>& projectile_manager,
    doubly_linked_list<Pick_Up_Item*>& pick_up_manager) 
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
                    layout[next_array_index] = converted_num_to_tile(next_tile);
                    spawn_entity_or_colliders(next_tile, next_array_index, enemy_manager, player_manager, projectile_manager, pick_up_manager);
                    next_array_index++;
                }
            }
            next_data_is_tile = !next_data_is_tile;
        }
    }
    infile.close();
}

int TileMap::converted_num_to_tile(const int& num) const {
    if (num == 1) {
        return 13; // 13 is a wall on the tilemap
    } else {
        return 82; // 82 is the ground on the tilemap
    }
}

void TileMap::spawn_entity_or_colliders(const int& num, const int& next_array_index, EnemyManager& enemy_manager, const PlayerManager& player_manager, doubly_linked_list<Projectile*>& projectile_manager, doubly_linked_list<Pick_Up_Item*>& pick_up_manager) {
    Vec2f spawn_location{
        static_cast<float>((next_array_index % width_in_tiles) * tile_width_pixels), 
        static_cast<float>((next_array_index / width_in_tiles) * tile_height_pixels)
    };
    EnemySpawner* new_enemy;
    Pick_Up_Item* new_pick_up;
        
    switch (num) {
    case 1:
        colliders.append(new Rect_Collider(tile_width_pixels, tile_height_pixels, spawn_location, camera));
        break;
    case 2:
        new_enemy = new EnemySpawner(spawn_location, camera, player_manager, enemy_manager, projectile_manager, Entity_Data(E_Barbarian));
        enemy_manager.add(new_enemy);
        break;
    case 3:
        new_enemy = new EnemySpawner(spawn_location, camera, player_manager, enemy_manager, projectile_manager, Entity_Data(E_Goblin));
        enemy_manager.add(new_enemy);
        break;
    case 4:
        new_pick_up = new Finish_Ladder(spawn_location, camera);
        pick_up_manager.append(new_pick_up);
        break;
    }
}

