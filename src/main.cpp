#include <iostream>
#include "world.hpp"
#include "logger.hpp"
#include "entity.hpp"
#include "species.hpp"
#include "flock.hpp"

int main(int argc, char** argv) {
    Logger logger;
    World world;

    Rabbit rabbit_species;
    Fox fox_species;


    Entity rabbit_entity(&rabbit_species);
    Entity another_rabbit(&rabbit_species);
    Entity fox_entity(&fox_species);


    Flock<Rabbit> rabbit_flock;
    Flock<Fox> fox_flock;

    rabbit_flock += rabbit_entity;        
    rabbit_flock += another_rabbit;       
    rabbit_flock += fox_entity;
    
    fox_flock += fox_entity;
    fox_flock += rabbit_entity;


    rabbit_flock.leader = &rabbit_entity;
    fox_flock.leader = &fox_entity;


    Flock<Rabbit> another_rabbit_flock;
    Flock<Rabbit> merged_rabbit_flock = rabbit_flock + another_rabbit_flock;

    return 0;
}