#pragma once
#include "GameEntities.h"

struct PlayerManager
{
    static constexpr unsigned int max_players = 4;
    HeroEntity* players[max_players]{};
    unsigned int player_count = 0;
    
    bool add(HeroEntity* player) {
        if (player_count >= max_players) {return false;}
        players[player_count] = player;
        player_count++;
        
        return true;
    }
    
    unsigned int get_player_count() const { return player_count; }
    unsigned int get_max_players() const { return max_players; }
};
