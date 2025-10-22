#include <crossguid/guid.hpp>
#include "entity.hpp"


Entity::Entity(Species* species_ptr) 
    : id(xg::newGuid()), species(species_ptr), age(0), energy(0.0f), is_alive(true)
{
    PLOG_DEBUG << "New entity created with uuid = {" << id << "} and species";
}

bool Entity::operator==(const Entity& other) const {
    return id.str() == other.id.str();
}