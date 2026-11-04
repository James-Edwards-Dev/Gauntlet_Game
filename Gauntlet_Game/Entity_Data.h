#pragma once

enum Entity_Type
{
    P_Knight,
    P_Rogue,
    P_Wizard,
    E_Goblin
};

struct Entity_Data
{
    float melee_damage = 50;
    float projectile_damage = 50;
    float projectile_speed = 100;
    float projectile_rate_of_fire = 1;
    float max_health = 100;
    float defense = 10;
    float speed = 50;
    
    Entity_Data(Entity_Type type) 
    {
        switch (type) {
            case P_Knight:
                melee_damage = 70;
                projectile_damage = 40;
                speed = 60;
                defense = 40; // Knight has a buff for defense also
                break;
            
            case P_Rogue:
                projectile_damage = 60;
                projectile_speed = 150;
                projectile_rate_of_fire = 1.5;
                speed = 100;
                break;
            
            case P_Wizard:
                projectile_damage = 100;
                projectile_speed = 110;
                projectile_rate_of_fire = 1.25;
                defense = 0;
                speed = 65;
                break;
            
            case E_Goblin:
                break;
        }
    }
};