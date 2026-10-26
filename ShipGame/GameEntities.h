#pragma once
#include "Entity.h"
#include <iostream>

#include "GERenderer.h"
#include "World.h"

class SpriteEntity : public VisualEntity
{
protected:
    GamesEngineeringBase::Image image;
public:
    SpriteEntity(const Vec2f& _position, const std::string& image_filename) : VisualEntity(_position)
    {
        image.load(image_filename);
    }
    virtual void draw(GamesEngineeringBase::Window& window) override {
        GERenderer::render_image(window, image, get_position());
    }
    const GamesEngineeringBase::Image& get_image() const {return image;}
};

class WorldSpriteEntity : public SpriteEntity
{
protected:
    const World& world;
public:
    WorldSpriteEntity(const Vec2f& _position, const std::string& image_filename, const World& _world) : SpriteEntity(_position, image_filename), world(_world) {}
    
    virtual void draw(GamesEngineeringBase::Window& window) override {
        GERenderer::render_image(window, get_image(), get_position() - world.get_offset());
    }
};

class HeroEntity : public WorldSpriteEntity
{
public:
    HeroEntity(const Vec2f& _position, const World& _world) : WorldSpriteEntity(_position, "resources/IdleAnimation2.png", _world) {}
    
    void set_input_axis(const Vec2f& input) {inputAxis = input;}
    const Vec2f& get_input_axis() const {return inputAxis;}
    
    virtual void update(float delta_time) override {
        move(get_input_axis() * speed * delta_time);
    }
    
protected:
    float speed = 100.0f;
    Vec2f inputAxis{0.0f, 0.0f};
};

class TileMap : public WorldSpriteEntity
{
private:
    unsigned int size_x = 32;
    unsigned int size_y = 32;
    unsigned int tiles_in_row;

public:
    TileMap(const Vec2f& _position, const World& _world) : WorldSpriteEntity(_position, "resources/tileset.png", _world) 
    {
        tiles_in_row = image.width / size_x;
    }
    virtual void update(float delta_time) override {
        // TileMap will not do anything when updating for now
    }
    void draw_tile(GamesEngineeringBase::Window& window, const Vec2f& location, unsigned int tile_index) {
        GERenderer::render_tile(window, get_image(), location, tile_index, size_x, size_y, tiles_in_row);
    }
    
private:
    
};