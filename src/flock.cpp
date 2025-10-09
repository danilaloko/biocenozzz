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

Flock Flock::operator+(const Flock& other) const {
    Flock result;

    PLOG_DEBUG << "Flocks {" << this->id << "} and {" << other.id << "} merged into created flock {" << result.id << "}";
    
    for (const Entity& entity : this->members) {
        result += entity;
    }
    
    for (const Entity& entity : other.members) {
        result += entity;
    }
    
    // we choose leader of flock from the first argument
    // TODO: rethink logic of new leader allignment
    if (this->leader != nullptr) {
        auto it = std::find(result.members.begin(), result.members.end(), *(this->leader));
        if (it != result.members.end()) {
            result.leader = &(*it);
        }
    } else if (other.leader != nullptr) {
        auto it = std::find(result.members.begin(), result.members.end(), *(other.leader));
        if (it != result.members.end()) {
            result.leader = &(*it);
        }
    }
    
    result.energy = this->energy + other.energy;
    
    return result;
}