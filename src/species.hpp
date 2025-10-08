#pragma once

#include <string>
#include "reproduction_strategy.hpp"

enum class TrophicCategory { 
    Producer, 
    Herbivore, 
    Carnivore, 
    Omnivore 
};

class Species{
public:
    //std::string name;
    TrophicCategory category;
    int lifespan;
    float max_energy;
    float sense_radius;
    float act_radius;
    float speed;
    //std::vector<ReproductionStrategy> reproduction_strategies;
};

class Rabbit : public Species{
    TrophicCategory category = Herbivore;
    int lifespan = 2;
    float max_energy = 1;
    float sense_radius = 5.0;
    float act_radius = 2.5;
    float speed = 7.0;
}
