#pragma once

#include <vector>
#include <memory>
#include <crossguid/guid.hpp>
#include "entity.hpp"

class Flock{
public:
    xg::Guid id;
    std::vector<std::shared_ptr<Entity>> members;
    std::shared_ptr<Entity> leader;
    float energy;

    Flock();
    ~Flock();

    void operator+=(std::shared_ptr<Entity> entity); 
    void operator-=(std::shared_ptr<Entity> entity);
    
    Flock operator+(Flock& other);
};