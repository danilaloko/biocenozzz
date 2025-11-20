#pragma once

#include <string>
#include <vector>
#include <memory>
#include "reproduction_strategy.hpp"

class Species;

enum class TrophicCategory { 
    Producer, 
    Herbivore, 
    Carnivore, 
    Omnivore 
};

class Species{
public:
    virtual ~Species() = default;
    TrophicCategory category;
    int lifespan;
    float max_energy;
    float sense_radius;
    float act_radius;
    float speed;
    std::vector<Species*> diet;
    std::vector<Species*> predators;
};

class Rabbit : public Species{
public:
    Rabbit() {
        category = TrophicCategory::Herbivore;
        lifespan = 2;
        max_energy = 1.0f;
        sense_radius = 5.0f;
        act_radius = 2.5f;
        speed = 7.0f;
    }
};

class Fox : public Species{
public:
    Fox() {
        category = TrophicCategory::Carnivore;
        lifespan = 5;
        max_energy = 1.0f;
        sense_radius = 30.0f;
        act_radius = 3.5f;
        speed = 7.0f;
    }
};

class Grass : public Species {
public:
    Grass() {
        category = TrophicCategory::Producer;
        lifespan = 2;
        max_energy = 0.0f;
        sense_radius = 1.0f;
        act_radius = 1.0f;
        speed = 0.0f;
    }
};