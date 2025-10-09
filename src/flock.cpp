#include <crossguid/guid.hpp>
#include "flock.hpp"


Flock::Flock()
    : id(xg::newGuid())
{
    PLOG_DEBUG << "New flock created with uuid = {" << id << "}";
}


Flock& Flock::operator+=(const Entity& entity) {
    if (std::find(members.begin(), members.end(), entity) == members.end()) {
        members.push_back(entity);
        PLOG_DEBUG << "Entity {" << entity.id << "} added to flock {" << id << "}";
    } else {
        PLOG_WARNING << "Entity {" << entity.id << "} already in flock {" << id << "}";
    }
    return *this;
}