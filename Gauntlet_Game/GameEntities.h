#pragma once
#include "Entity.h"
#include <iostream>

#include "GERenderer.h"
#include "Camera.h"
#include "Collision.h"
#include "Entity_Data.h"

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
    virtual void update(float delta_time) override {
        // Do nothing
    }
    const GamesEngineeringBase::Image& get_image() const {return image;}
    
    Vec2f get_centre() const {
        Vec2f size =  Vec2f(static_cast<float>(get_image().width), static_cast<float>(get_image().height));
        return get_position() + size / 2;
    }
};

class WorldSpriteEntity : public SpriteEntity
{
protected:
    const Camera& camera;
public:
    WorldSpriteEntity(const Vec2f& _position, const std::string& image_filename, const Camera& _camera) : SpriteEntity(_position, image_filename), camera(_camera) {}
    
    virtual void draw(GamesEngineeringBase::Window& window) override {
        GERenderer::render_image(window, image, get_position() - camera.camera_offset);
    }
};

class CollisionSprite : public WorldSpriteEntity
{
protected:
    Rect_Collider collider;

public:
    CollisionSprite(const Vec2f& _position, const std::string& image_filename, const Camera& _camera) 
    : WorldSpriteEntity(_position, image_filename, _camera)
    , collider(get_image().width, get_image().height, _position, _camera) {}
    
    const Rect_Collider& get_collider() const{
        return collider;
    }
    
    void draw_collider(GamesEngineeringBase::Window& window) const {
        collider.draw_debug_lines(window);
    }
};