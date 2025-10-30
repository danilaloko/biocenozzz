#include <QCoreApplication>
#include <iostream>
#include "core/world.hpp"
#include "core/logger.hpp"
#include "core/entity.hpp"
#include "core/species.hpp"
#include "core/flock.hpp"
#include "core/SpatialSubdivision/spatial_subdiv_solver.hpp"

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    
    Logger logger;
    World* world = new World(100, 100);
    
    // Создаем solver как указатель
    SpatialSubdivSolver* solver = new SpatialSubdivSolver(*world);

    Rabbit rabbit_species;
    Fox fox_species;

    Entity* rabbit1 = new Entity(&rabbit_species);

    world->entity_map.insert(rabbit1->id, rabbit1);

    // СОЕДИНЕНИЕ - передаем указатель на solver
    QObject::connect(rabbit1, &Entity::update_pos_signal,
                    solver, &SpatialSubdivSolver::pos_update_handler);
    
    rabbit1->update_pos(10.5, 20.3);
    
    return app.exec();
}