#include <QCoreApplication>
#include <iostream>
#include "core/world.hpp"
#include "core/logger.hpp"
#include "core/entity.hpp"
#include "core/species.hpp"
#include "core/flock.hpp"

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    
    Logger logger;
    World world;

    Rabbit rabbit_species;
    Fox fox_species;


    Entity* rabbit1 = new Entity(&rabbit_species);
    Entity* rabbit2 = new Entity(&rabbit_species);

    Entity* fox = new Entity(&fox_species);


    Flock<Rabbit> rabbit_flock;
    Flock<Fox> fox_flock;

    rabbit_flock += rabbit1;        
    rabbit_flock += rabbit2;   

    rabbit_flock += fox;    
    fox_flock += fox;


    rabbit_flock.leader = rabbit1;
    fox_flock.leader = fox;

    return 0;
}