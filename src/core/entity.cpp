#include <QUuid>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "entity.hpp"
#include <algorithm>

Entity::Entity(Species* species_ptr) 
    : QObject(), id(QUuid::createUuid()), species(species_ptr), age(0), 
      energy(species_ptr->max_energy), is_alive(true), x(0.0), y(0.0), 
      _state(State::idle), _target_pos_x(0.0), _target_pos_y(0.0)
{
    visible_entities.clear();
    _target_pos_x = x;
    _target_pos_y = y;
    
    PLOG_DEBUG << "New entity created with uuid = {" << id.toString().toStdString() 
               << "} at position (" << x << ", " << y << ") with energy: " << energy;
}


void Entity::update() {
    energy -= 0.01f;
    if (energy <= 0.0f) {
        die();
        return;
    }
    
    _check_energy();
    
    switch (_state) {
        case State::idle:
            _update_idle();
            break;
            
        case State::searching_for_food:
            _update_searching_for_food();
            break;
    }
    
    _move_to_target();
}

void Entity::_check_energy() {
    float hunger_threshold = species->max_energy * 0.75f;
    
    if (energy <= hunger_threshold && _state != State::searching_for_food) {
        _state = State::searching_for_food;
        PLOG_DEBUG << "Entity " << id.toString().toStdString() 
                   << " is now hungry (energy: " << energy << ") and starts searching for food";
    } else if (energy > hunger_threshold && _state == State::searching_for_food) {
        _state = State::idle;
        PLOG_DEBUG << "Entity " << id.toString().toStdString() 
                   << " is no longer hungry (energy: " << energy << ") and returns to idle";
    }
}

Entity* Entity::_find_food() {
    for (Entity* other : visible_entities) {
        if (_can_eat(other)) {
            return other;
        }
    }
    return nullptr;
}

void Entity::_find_predators() {
    // TODO
}

void Entity::_update_searching_for_food() {
    Entity* food_target = _find_food();
    
    if (food_target) {
        _target_pos_x = food_target->x;
        _target_pos_y = food_target->y;
        
        PLOG_DEBUG << "Entity " << id.toString().toStdString() 
                   << " moving towards food at (" << _target_pos_x << ", " << _target_pos_y << ")";
    } else {
        double angle = (std::rand() % 360) * 3.14 / 180.0;
        double distance = species->sense_radius;
        
        _target_pos_x = x + std::cos(angle) * distance;
        _target_pos_y = y + std::sin(angle) * distance;
        
        _target_pos_x = std::max(0.0, std::min(_target_pos_x, 100.0));
        _target_pos_y = std::max(0.0, std::min(_target_pos_y, 100.0));
        
        PLOG_DEBUG << "Entity " << id.toString().toStdString() 
                   << " no food found, exploring to (" << _target_pos_x << ", " << _target_pos_y << ")";
    }
}


bool Entity::_can_eat(Entity* other) const {
    auto it = std::find(species->diet.begin(), species->diet.end(), other->species);
    return it != species->diet.end();
}

bool Entity::_is_threat(Entity* other) const {
    auto it = std::find(other->species->diet.begin(), other->species->diet.end(), this->species);
    return it != other->species->diet.end();
}

void Entity::update_pos(double target_x, double target_y) {
    x = target_x;
    y = target_y;

    emit update_pos_signal(id, x, y);
    emit entity_moved_signal(this); // НОВЫЙ сигнал с указателем
    
    // PLOG_DEBUG << "Entity " << id.toString().toStdString() 
    //            << " moved to (" << x << ", " << y << ")";
}

void Entity::on_other_entity_moved(QUuid other_id, double other_x, double other_y) {
    if (other_id == this->id) {
        return;
    }

    double distance = std::sqrt(std::pow(this->x - other_x, 2) + 
                              std::pow(this->y - other_y, 2));
    
    if (distance <= species->sense_radius) {
        PLOG_DEBUG << "!!! Entity " << this->id.toString().toStdString() 
                   << " DETECTED entity " << other_id.toString().toStdString() 
                   << " at distance " << distance;
    }
}

void Entity::on_other_entity_moved_ptr(Entity* other_entity) {
    if (other_entity->id == this->id) {
        return;
    }

    double distance = std::sqrt(std::pow(this->x - other_entity->x, 2) + 
                              std::pow(this->y - other_entity->y, 2));
    
    if (distance <= species->sense_radius) {
        if (std::find(visible_entities.begin(), visible_entities.end(), other_entity) == visible_entities.end()) {
            visible_entities.push_back(other_entity);
            PLOG_DEBUG << "Entity " << this->id.toString().toStdString() 
                       << " ADDED to visible: " << other_entity->id.toString().toStdString()
                       << " at distance " << distance;
        }
    } else {
        auto it = std::find(visible_entities.begin(), visible_entities.end(), other_entity);
        if (it != visible_entities.end()) {
            visible_entities.erase(it);
            PLOG_DEBUG << "Entity " << this->id.toString().toStdString() 
                       << " REMOVED from visible: " << other_entity->id.toString().toStdString()
                       << " (distance: " << distance << ")";
        }
    }
}

void Entity::die() {
    is_alive = false;
    PLOG_DEBUG << "Entity " << id.toString().toStdString() << " died.";
}

void Entity::_update_idle() {
    double distance_to_target = std::sqrt(std::pow(x - _target_pos_x, 2) + 
                                        std::pow(y - _target_pos_y, 2));
    
    if (distance_to_target < 1.0) {
        double angle = (std::rand() % 360) * 3.14 / 180.0;
        double distance = (std::rand() % static_cast<int>(species->sense_radius));
        
        _target_pos_x = x + std::cos(angle) * distance;
        _target_pos_y = y + std::sin(angle) * distance;
        
        _target_pos_x = std::max(0.0, std::min(_target_pos_x, static_cast<double>(100)));
        _target_pos_y = std::max(0.0, std::min(_target_pos_y, static_cast<double>(100)));
        
        PLOG_DEBUG << "Entity " << id.toString().toStdString() 
                   << " new target: (" << _target_pos_x << ", " << _target_pos_y << ")";
    }
}

void Entity::_move_to_target() {
    double direction_x = _target_pos_x - x;
    double direction_y = _target_pos_y - y;
    double length = std::sqrt(direction_x * direction_x + direction_y * direction_y);
    
    if (length < 0.5) {
        return;
    }

    direction_x /= length;
    direction_y /= length;
    
    double new_x = x + direction_x * species->speed * 0.1;
    double new_y = y + direction_y * species->speed * 0.1;
    
    update_pos(new_x, new_y);
}

void Entity::_eat() {
}

void Entity::_reproduce() {
}

void Entity::_attack() {
}

void Entity::_sense() {
}