#include <iostream>
#include "world.hpp"
#include "logger.hpp"

// for testing
#include "entity.hpp"
#include "flock.hpp"

int main(int argc, char** argv) {
    Logger logger;
    World world;

    Entity entity0;
    Entity entity1;
    Flock flock0;
    flock0.leader = &entity0;

    flock0 -= entity0;
    flock0 += entity0;
    flock0 += entity1;
    flock0 -= entity0;

    return 0;
}
