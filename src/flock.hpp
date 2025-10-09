#pragma once

#include <vector>
#include <crossguid/guid.hpp>
#include "entity.hpp"

class Flock{
public:
    xg::Guid const id;
    std::vector<Entity> members;
    Entity *leader;
    float energy;

    Flock();

    Flock& operator+=(const Entity& entity); 
};