#pragma once

#include "../world.hpp"

class SpatialSubdivSolver {

public:
    SpatialSubdivSolver(World &world);

private:
    World &world;
};