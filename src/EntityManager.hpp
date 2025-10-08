#pragma once

#include <vector>
#include "entity.hpp"

class EntityManager {
public:
    std::vector<Entity> entities;

    void update();
};