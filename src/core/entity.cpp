#include <QUuid>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "entity.hpp"

Entity::Entity(Species* species_ptr) 
    : QObject(), id(QUuid::createUuid()), species(species_ptr), age(0), energy(0.0f), 
      is_alive(true), x(0.0), y(0.0), _state(State::idle),
      _target_pos_x(0.0), _target_pos_y(0.0)
{
    visible_entities.clear();
    _target_pos_x = x;
    _target_pos_y = y;
    
    PLOG_DEBUG << "New entity created with uuid = {" << id.toString().toStdString() 
               << "} at position (" << x << ", " << y << ")";
}

void Entity::update() {
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

void Entity::_update_searching_for_food() {
    // TODO
    _update_idle();
}

bool Entity::operator==(const Entity& other) const {
    return id == other.id;
}

void Entity::update_pos(double target_x, double target_y) {
    x = target_x;
    y = target_y;

    // PLOG_DEBUG << "Entity " << id.toString().toStdString() 
    //            << " moved to (" << x << ", " << y << ")";
    emit update_pos_signal(id, x, y);
}

void Entity::on_other_entity_moved(QUuid other_id, double other_x, double other_y) {
    if (other_id == this->id) {
        return;
    }

    double distance = std::sqrt(std::pow(this->x - other_x, 2) + 
                              std::pow(this->y - other_y, 2));
    
    // PLOG_DEBUG << "Entity " << this->id.toString().toStdString() 
    //            << " checking distance to " << other_id.toString().toStdString() 
    //            << ": " << distance << " (sense radius: " << species->sense_radius << ")";

    if (distance <= species->sense_radius) {
        PLOG_DEBUG << "!!! Entity " << this->id.toString().toStdString() 
                   << " DETECTED entity " << other_id.toString().toStdString() 
                   << " at distance " << distance;
    }
}