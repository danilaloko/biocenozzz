#pragma once

#include <vector>
#include <crossguid/guid.hpp>
#include "entity.hpp"

class Flock{
public:
    xg::Guid id;
    std::vector<Entity> members;
    Entity *leader;
    float energy;

    Flock();

    void operator+=(const Entity& entity); 
    void operator-=(const Entity& entity); 
    Flock operator+(const Flock& other) const;
};