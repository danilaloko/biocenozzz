#include <crossguid/guid.hpp>
#include <algorithm>
#include "flock.hpp"

Flock::Flock()
    : id(xg::newGuid()), energy(0.0f)
{
    PLOG_DEBUG << "New flock created with uuid = {" << id << "}";
}

Flock::~Flock() {
    PLOG_DEBUG << "Flock destroyed with uuid = {" << id << "}";
}

void Flock::operator+=(std::shared_ptr<Entity> entity) {
    auto it = std::find_if(members.begin(), members.end(),
        [&](const std::shared_ptr<Entity>& e) { 
            return e->id == entity->id; 
        });
    
    if (it == members.end()) {
        members.push_back(entity);
        PLOG_DEBUG << "Entity {" << entity->id << "} added to flock {" << id << "}";
    } else {
        PLOG_ERROR << "Tried to add entity {" << entity->id << "} to flock {" << id << "}, but it's already in flock";
    }
}

void Flock::operator-=(std::shared_ptr<Entity> entity) {
    auto it = std::find_if(members.begin(), members.end(),
        [&](const std::shared_ptr<Entity>& e) { 
            return e->id == entity->id; 
        });
    
    if (it != members.end()) {
        members.erase(it);
        
        if (leader && leader->id == entity->id) {
            leader = nullptr;
            PLOG_DEBUG << "Leader Entity {" << entity->id << "} was removed from flock {" << id << "}";
        } else {
            PLOG_DEBUG << "Entity {" << entity->id << "} removed from flock {" << id << "}";
        }
    } else {
        PLOG_ERROR << "Tried to remove entity {" << entity->id << "} from flock {" << id << "}, but not found it there";
    }
}

Flock Flock::operator+(Flock& other) {
    Flock result;
    
    for (auto& entity : this->members) {
        result += entity;
    }
    
    for (auto& entity : other.members) {
        result += entity;
    }
    
    if (this->leader) {
        result.leader = this->leader;
    } else if (other.leader) {
        result.leader = other.leader;
    }
    
    result.energy = this->energy + other.energy;
    
    this->members.clear();
    this->leader = nullptr;
    this->energy = 0.0f;
    
    other.members.clear();
    other.leader = nullptr;
    other.energy = 0.0f;
    
    PLOG_DEBUG << "Flocks merged and original flocks cleared. New flock: {" << result.id << "}";
    
    return result;
}