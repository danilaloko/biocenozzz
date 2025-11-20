#pragma once

#include <string>
#include <vector>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include "reproduction_strategy.hpp"

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
    std::vector<std::type_index> diet;
    
    virtual std::type_index get_type() const = 0;
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
    
    std::type_index get_type() const override {
        return std::type_index(typeid(Grass));
    }
};

class Rabbit : public Species{
public:
    Rabbit() {
        category = TrophicCategory::Herbivore;
        lifespan = 2;
        max_energy = 1.0f;
        sense_radius = 100.0f;
        act_radius = 2.5f;
        speed = 7.0f;
        diet.push_back(std::type_index(typeid(Grass)));
    }
    
    std::type_index get_type() const override {
        return std::type_index(typeid(Rabbit));
    }
};

class Fox : public Species{
public:
    Fox() {
        category = TrophicCategory::Carnivore;
        lifespan = 5;
        max_energy = 1.0f;
        sense_radius = 150.0f;
        act_radius = 3.5f;
        speed = 7.0f;
        diet.push_back(std::type_index(typeid(Rabbit)));
    }
    
    std::type_index get_type() const override {
        return std::type_index(typeid(Fox));
    }
};