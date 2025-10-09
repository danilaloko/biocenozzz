#pragma once 

#include <vector>
#include <crossguid/guid.hpp>
#include "logger.hpp"
//#include "species.hpp"

class Entity{
public:
    xg::Guid const id;
    //Species species;
    int age;
    float energy;
    std::vector<Entity> children;
    bool is_alive;
    double x;
    double y;

    //Entity(Species *Species);
    Entity();
    void update();
    void die();

private:
    void _move();
    void _eat();
    void _reproduce();
    void _attack();
    void _sense();
};