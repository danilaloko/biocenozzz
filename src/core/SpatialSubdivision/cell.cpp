#include "cell.hpp"
#include <iostream>

Cell::Cell(double cell_size) : size(cell_size) {
}

void Cell::add_entity(Entity* entity) {
    entity_map.insert(entity->id, entity);
    PLOG_DEBUG << "Entity " << entity->id.toString().toStdString() 
               << " added to cell";
}

void Cell::remove_entity(QUuid id) {
    entity_map.remove(id);
    PLOG_DEBUG << "Entity " << id.toString().toStdString() 
               << " removed from cell";
}