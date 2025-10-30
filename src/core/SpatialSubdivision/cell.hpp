#pragma once

#include <QHash>
#include <QUuid>
#include "../entity.hpp"

class Cell {
public:
    QHash<QUuid, Entity*> entity_map;
    double size;

    Cell(double cell_size);

    void add_entity(Entity* entity);
    void remove_entity(QUuid id);
};