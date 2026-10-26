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
        GERenderer::draw_image(window, image, get_position());
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
        GERenderer::draw_image(window, get_image(), get_position() - world.get_offset());
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

class TitleMap : public WorldSpriteEntity
{
private:
    unsigned int size_x = 32;
    unsigned int size_y = 32;

public:
    TitleMap(const Vec2f& _position, const World& _world) : WorldSpriteEntity(_position, "resources/tileset.png", _world) {
        unsigned int size = (image.width / size_x) * (image.height / size_y);
    }
    void draw_tile(GamesEngineeringBase::Window& window, Vec2f location, unsigned int tile_x, unsigned int tile_y) 
    {
        unsigned int start_x = tile_x * size_x;
        unsigned int start_y = tile_y * size_y;
        
        for (unsigned int col = start_y; col < start_y + 32; col++) {
            
            unsigned int draw_y = static_cast<int>(location.y) + col - start_y;
            
            for (unsigned int row = start_x; row < start_x + 32; row++) {
                
                unsigned int draw_x = static_cast<int>(location.x) + row - start_x;
                
                const unsigned int pixelIndex = (col * image.width + row) * image.channels;
                
                if (image.channels == 3 || image.data[pixelIndex + 3] != 0) {
                    window.draw(draw_x, draw_y, &image.data[pixelIndex]);
                }
            }
        }
    }
    virtual void update(float delta_time) override {
        // TileMap will not do anything when updating for now
    }
    
private:
    
};