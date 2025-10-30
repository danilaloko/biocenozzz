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
    SpatialSubdivSolver solver(*world);
    

    return 0;
}

