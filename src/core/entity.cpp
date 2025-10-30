#include <QUuid>
#include <cmath>
#include "entity.hpp"

Entity::Entity(Species* species_ptr) 
    : QObject(), id(QUuid::createUuid()), species(species_ptr), age(0), energy(0.0f), 
      is_alive(true), x(0.0), y(0.0)
{
    visible_entities.clear();
    PLOG_DEBUG << "New entity created with uuid = {" << id.toString().toStdString() << "} and species";
}

bool Entity::operator==(const Entity& other) const {
    return id == other.id;
}

void Entity::update_pos(double target_x, double target_y) {
    x = target_x;
    y = target_y;

    emit update_pos_signal(id, x, y);
    PLOG_DEBUG << "Entity " << id.toString().toStdString() << " emited signal update_pos_signal";
}