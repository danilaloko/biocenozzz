#include "spatial_subdiv_solver.hpp"
#include <iostream>

SpatialSubdivSolver::SpatialSubdivSolver(World &world)
: world(world)
{
    std::cout << "Hello world!" << std::endl;
    PLOG_DEBUG << "Spatial subdivision solver initialized. World sizes are: x=" << world.size_x << ", y=" << world.size_y;
}