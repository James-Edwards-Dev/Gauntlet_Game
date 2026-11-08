#pragma once

#include "Collision.h"
#include "Data_Structs.h"
#include "Enemy_Manager.h"
#include "GameEntities.h"
#include "Pick_Up_Item.h"

class TileMap : public WorldSpriteEntity
{
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
    TileMap(const Vec2f& _position, const Camera& _camera);
    
    ~TileMap();
    
    virtual void draw(GamesEngineeringBase::Window& window) override;
    virtual void update(float delta_time) override;
    
    void draw_colliders(GamesEngineeringBase::Window& window, unsigned char r=255, unsigned char g=0, unsigned char b=0);
    
    void block(SpriteEntity& entity, const Rect_Collider& collider);
    
    bool is_overlapping(const Rect_Collider& collider);
    
    void load(std::string level_filepath, EnemyManager& enemy_manager, const PlayerManager& player_manager, doubly_linked_list<Projectile*>& projectile_manager, doubly_linked_list<Pick_Up_Item*>& pick_up_manager);
    
protected:
    int converted_num_to_tile(const int& num) const; 
    
    void spawn_entity_or_colliders(
        const int& num, 
        const int& next_array_index, 
        EnemyManager& enemy_manager, 
        const PlayerManager& player_manager, 
        doubly_linked_list<Projectile*>& projectile_manager,
        doubly_linked_list<Pick_Up_Item*>& pick_up_manager);


};

