#include <crossguid/guid.hpp>
#include "entity.hpp"

Entity::Entity() 
    : id(xg::newGuid()), age(0), energy(0.0f), is_alive(true), x(0.0), y(0.0)
{
    PLOG_DEBUG << "New entity created with uuid = {" << id << "}";
}

Entity::~Entity() {
    PLOG_DEBUG << "Entity destroyed with uuid = {" << id << "}";
}

bool Entity::operator==(const Entity& other) const {
    return id == other.id;
}