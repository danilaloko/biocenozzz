#pragma once

#include <QObject>
#include "world.hpp"
#include "entity.hpp"
#include "species.hpp"

class EntityFactory : public QObject {
    Q_OBJECT

public:
    EntityFactory(World* world_ptr);
    Entity* create_entity(Species* species_ptr, double x = 0.0, double y = 0.0);

private:
    World* world;
};