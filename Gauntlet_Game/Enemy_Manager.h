#pragma once

#include "CombatSprite.h"
#include "Enemy.h"
#include "TileMap.h"

class EnemyManager
{
public:
    doubly_linked_list<Enemy*> Enemys_List;
    
    void add(Enemy* enemy) {
        Enemys_List.append(enemy);
    }
    void update(float delta_time) {
        doubly_node<Enemy*>* node = Enemys_List.head;
        while (node != nullptr) {
            doubly_node<Enemy*>* next_node = node->next;
            Enemy* enemy = node->data;
            enemy->update(delta_time);
            
            if (!enemy->is_alive()) {
                Enemys_List.remove(node);
            }
            node = next_node;
        }
    }
    void draw(GamesEngineeringBase::Window& window) const {
        for (doubly_node<Enemy*>* node = Enemys_List.head; node != nullptr; node = node->next) {
            Enemy* enemy = node->data;
            enemy->draw(window);
        }
    }
    void draw_colliders(GamesEngineeringBase::Window& window) const {
        for (doubly_node<Enemy*>* node = Enemys_List.head; node != nullptr; node = node->next) {
            Enemy* enemy = node->data;
            enemy->get_collider().draw_debug_lines(window);
        }
    }
};
