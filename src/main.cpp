#include <QCoreApplication>
#include <QTimer>

#include <iostream>
#include "core/world.hpp"
#include "core/logger.hpp"
#include "core/entity.hpp"
#include "core/species.hpp"
#include "core/SpatialSubdivision/spatial_subdiv_solver.hpp"

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    
    Logger logger;
    World* world = new World(100, 100);
    
    SpatialSubdivSolver* solver = new SpatialSubdivSolver(*world, 5.0);

    Rabbit rabbit_species;

    Entity* rabbit1 = new Entity(&rabbit_species);
    Entity* rabbit2 = new Entity(&rabbit_species);

    world->entity_map.insert(rabbit1->id, rabbit1);
    world->entity_map.insert(rabbit2->id, rabbit2);

    // TODO вот это непотребство надо в фабрику убрать
    QObject::connect(rabbit1, &Entity::update_pos_signal,
                    solver, &SpatialSubdivSolver::pos_update_handler);
    QObject::connect(rabbit2, &Entity::update_pos_signal,
                    solver, &SpatialSubdivSolver::pos_update_handler);
    

    std::cout << "Setting initial positions..." << std::endl;
    rabbit1->update_pos(2.0, 2.0);  
    rabbit2->update_pos(25.0, 25.0); 
    

    std::cout << "Moving rabbit1 closer to rabbit2..." << std::endl;

    rabbit1->update_pos(23.0, 23.0);

    auto collisions = solver->get_collisions_by_id(rabbit2->id);
    PLOG_DEBUG << "Rabbit2 {" << rabbit2->id.toString().toStdString() << "} can see " << collisions[0]->id.toString().toStdString();
            

    
    return app.exec();
}