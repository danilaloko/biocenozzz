#include <crossguid/guid.hpp>
#include "flock.hpp"


Flock::Flock()
    : id(xg::newGuid()), energy(0.0f)
{
    PLOG_DEBUG << "New flock created with uuid = {" << id << "}";
}


void Flock::operator+=(const Entity& entity) {
    if (std::find(members.begin(), members.end(), entity) == members.end()) {
        members.push_back(entity);
        PLOG_DEBUG << "Entity {" << entity.id << "} added to flock {" << id << "}";
    } else {
        PLOG_ERROR << "Tried to add entity {" << entity.id << "} to flock {" << id << "}, but it's already in flock";
    }
}

void Flock::operator-=(const Entity& entity) {
    auto it = std::find(members.begin(), members.end(), entity);
    
    if (it != members.end()) {
        members.erase(it);
        
        if (leader != nullptr && *leader == entity) {
            leader = nullptr; // TODO: new leader assignment
            PLOG_DEBUG << "Leader Entity {" << entity.id << "} was removed from flock {" << id << "}";
        } else {
            PLOG_DEBUG << "Entity {" << entity.id << "} removed from flock {" << id << "}";
        }
    } else {
        PLOG_ERROR << "Tried to remove entity {" << entity.id << "} from flock {" << id << "}, but not found it there";
    }
}