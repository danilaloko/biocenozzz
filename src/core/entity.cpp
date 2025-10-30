#include <QUuid>
#include <cmath>
#include "entity.hpp"

Entity::Entity(Species* species_ptr) 
    : QObject(), id(QUuid::createUuid()), species(species_ptr), age(0), energy(0.0f), 
      is_alive(true), x(0.0), y(0.0)
{
    PLOG_DEBUG << "New entity created with uuid = {" << id.toString().toStdString() << "} and species";
}

bool Entity::operator==(const Entity& other) const {
    return id == other.id;
}