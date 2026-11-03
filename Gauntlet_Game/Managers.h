#pragma once
#include "Data_Structs.h"
#include "Enemys.h"
#include "GameEntities.h"

class Enemy;

class PlayerManager
{
    static constexpr unsigned int max_players = 4;
    unsigned int player_count = 0;
    
public:
    HeroEntity* players[max_players]{};
    
    bool add(HeroEntity* player) {
        if (player_count >= max_players) {return false;}
        players[player_count] = player;
        player_count++;
        
        return true;
    }
    
    unsigned int get_player_count() const { return player_count; }
    unsigned int get_max_players() const { return max_players; }
};
