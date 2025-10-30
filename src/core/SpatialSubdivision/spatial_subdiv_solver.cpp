#include "spatial_subdiv_solver.hpp"
#include <QDebug>
#include <iostream>

SpatialSubdivSolver::SpatialSubdivSolver(World &world)
    : QObject(), world(world)
{
    PLOG_DEBUG << "Spatial subdivision solver initialized. World sizes are: x=" << world.size_x << ", y=" << world.size_y;
}

void SpatialSubdivSolver::pos_update_handler(QUuid id, double x, double y) {

    PLOG_DEBUG << "Entity position update - ID: " << id.toString().toStdString()
              << " X: " << x << " Y: " << y << std::endl;

}