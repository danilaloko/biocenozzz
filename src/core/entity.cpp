// entity.cpp
#include <QUuid>
#include <cmath>
#include "entity.hpp"

Entity::Entity(Species* species_ptr) 
    : QObject(), id(QUuid::createUuid()), species(species_ptr), age(0), energy(0.0f), 
      is_alive(true), x(0.0), y(0.0)
{
    visible_entities.clear();
    PLOG_DEBUG << "New entity created with uuid = {" << id.toString().toStdString() << "}";
}

bool Entity::operator==(const Entity& other) const {
    return id == other.id;
}

void Entity::update_pos(double target_x, double target_y) {
    x = target_x;
    y = target_y;

    PLOG_DEBUG << "Entity " << id.toString().toStdString() 
               << " moved to (" << x << ", " << y << ")";
    emit update_pos_signal(id, x, y);
}

void Entity::on_other_entity_moved(QUuid other_id, double other_x, double other_y) {
    if (other_id == this->id) {
        return;
    }

    double distance = std::sqrt(std::pow(this->x - other_x, 2) + 
                              std::pow(this->y - other_y, 2));
    
    PLOG_DEBUG << "Entity " << this->id.toString().toStdString() 
               << " checking distance to " << other_id.toString().toStdString() 
               << ": " << distance << " (sense radius: " << species->sense_radius << ")";

    if (distance <= species->sense_radius) {
        PLOG_DEBUG << "!!! Entity " << this->id.toString().toStdString() 
                   << " DETECTED entity " << other_id.toString().toStdString() 
                   << " at distance " << distance;
    }
}