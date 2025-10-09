#pragma once 

#include <vector>
#include <memory>
#include <crossguid/guid.hpp>
#include "logger.hpp"

class Entity{
public:
    xg::Guid id;
    int age;
    float energy;
    std::vector<std::shared_ptr<Entity>> children;
    bool is_alive;
    double x;
    double y;

    Entity();
    ~Entity();
    
    void update();
    void die();

    bool operator==(const Entity& other) const;

private:
    void _move();
    void _eat();
    void _reproduce();
    void _attack();
    void _sense();
};