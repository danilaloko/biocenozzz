#pragma once 

#include <vector>
#include "species.hpp"

class Entity{
public:
    Species species;
    int age;
    float energy;
    std::vector<Entity> children;
    bool is_alive;
    double x;
    double y;

    Entity(Species *Species);
    void update();
    void die();

private:
    void _move();
    void _eat();
    void _reproduce();
    void _attack();
    void _sense();
};