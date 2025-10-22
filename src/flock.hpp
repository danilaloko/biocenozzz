#pragma once

#include <vector>
#include <algorithm>
#include <crossguid/guid.hpp>
#include <typeinfo>
#include "entity.hpp"
#include "species.hpp"

template<typename SpeciesType>
class Flock {
public:
    xg::Guid id;
    std::vector<Entity> members;
    Entity* leader;
    float energy;

    Flock() : id(xg::newGuid()), leader(nullptr), energy(0.0f) {
        PLOG_DEBUG << "New typed flock created with uuid = {" << id << "}";
    }

    void operator+=(const Entity& entity) {
        if (entity.species != nullptr) {
            const SpeciesType* specific_species = dynamic_cast<const SpeciesType*>(entity.species);
            
            if (specific_species != nullptr) {
                if (std::find(members.begin(), members.end(), entity) == members.end()) {
                    members.push_back(entity);
                    PLOG_DEBUG << "Entity {" << entity.id << "} added to typed flock {" << id << "}";
                } else {
                    PLOG_ERROR << "Tried to add entity {" << entity.id << "} to typed flock {" << id << "}, but it's already in flock";
                }
            } else {
                PLOG_ERROR << "Tried to add entity {" << entity.id << "} to typed flock {" << id << "}, but species type doesn't match. "
                           << "Expected: " << typeid(SpeciesType).name() 
                           << ", Got: " << typeid(*entity.species).name();
            }
        } else {
            PLOG_ERROR << "Tried to add entity {" << entity.id << "} without species to typed flock";
        }
    }

    void operator-=(const Entity& entity) {
        auto it = std::find(members.begin(), members.end(), entity);
        
        if (it != members.end()) {
            members.erase(it);
            
            if (leader != nullptr && *leader == entity) {
                leader = nullptr;
                PLOG_DEBUG << "Leader Entity {" << entity.id << "} was removed from typed flock {" << id << "}";
            } else {
                PLOG_DEBUG << "Entity {" << entity.id << "} removed from typed flock {" << id << "}";
            }
        } else {
            PLOG_ERROR << "Tried to remove entity {" << entity.id << "} from typed flock {" << id << "}, but not found it there";
        }
    }

    Flock<SpeciesType> operator+(const Flock<SpeciesType>& other) const {
        Flock<SpeciesType> result;

        PLOG_DEBUG << "Typed flocks {" << this->id << "} and {" << other.id << "} merged into created flock {" << result.id << "}";
        
        for (const Entity& entity : this->members) {
            result += entity;
        }
        
        for (const Entity& entity : other.members) {
            result += entity;
        }
        
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
    
};