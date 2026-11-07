#pragma once
#include "Players.h"

class Enemy : public CombatSprite
{
protected:
    const PlayerManager player_manager;
public:
    Enemy(
        const Vec2f& _position, 
        const Camera& _camera, 
        const PlayerManager& _player_manager, 
        doubly_linked_list<Projectile*>& projectile_manager, 
        Entity_Data entity_data); 
    
    
    virtual void update(float delta_time) override;
    virtual void defeat() override;
    
    void move(float delta_time, HeroEntity* target);
    
private:
    // Returns the distance to a closest target
    HeroEntity* find_closest_hero() const;
};
