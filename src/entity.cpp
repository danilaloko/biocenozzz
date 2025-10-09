#include <crossguid/guid.hpp>
#include "entity.hpp"

Entity::Entity() 
    : id(xg::newGuid()), age(0), energy(0.0f), is_alive(true)
{
    PLOG_DEBUG << "New entity created with uuid = {" << id << "}";
}

bool Entity::operator==(const Entity& other) const {
    return id.str() == other.id.str();
}