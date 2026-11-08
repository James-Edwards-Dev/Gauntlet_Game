#pragma once

enum Entity_Type
{
    P_Warrior,
    P_Rogue,
    P_Wizard,
    E_Barbarian,
    E_Goblin,
    E_Spawner
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
    
    std::string sprite_img_filepath;
    std::string projectile_img_filepath = "resources/projectile.png";;
    
    Entity_Data(Entity_Type type) 
    {
        switch (type) {
            case P_Warrior:
                sprite_img_filepath = "resources/warrior.png";
                
                max_health = 500;
                melee_damage = 100;
                projectile_damage = 50;
                speed = 65;
                defense = 40; // Knight has a buff for defense also
                break;
            
            case P_Rogue:
                sprite_img_filepath = "resources/rogue.png";
            
                max_health = 500;
                projectile_damage = 70;
                projectile_speed = 170;
                projectile_rate_of_fire = 0.5;
                speed = 100;
                break;
            
            case P_Wizard:
                sprite_img_filepath = "resources/wizard.png";
            
                max_health = 500;
                melee_damage = 75;
                projectile_damage = 150;
                projectile_speed = 120;
                projectile_rate_of_fire = 0.75;
                defense = 5;
                speed = 70;
                break;
            
            case E_Goblin:
                sprite_img_filepath = "resources/goblin.png";
                defense = 0;
                break;
            
            case E_Barbarian:
                // Setting projectile_rate_of_fire to 0 will make this a Moving Enemy 
                // Instead of a ProjectileFiringEnemy when spawned
                sprite_img_filepath = "resources/barbarian.png";
                projectile_rate_of_fire = 0;
                speed = 45;
                break;
            
            case E_Spawner:
                sprite_img_filepath = "resources/warrior.png";
                melee_damage = 0;
                max_health = 80;
                defense = 0;
                break;
        }
    }
};



struct Spawner_Data
{
    Entity_Data spawning_entity_;
    float spawn_time = 2.0f;
    unsigned int max_enemies = 6;
    
    Spawner_Data(const Entity_Data& spawning_entity) : spawning_entity_(spawning_entity) {}
};