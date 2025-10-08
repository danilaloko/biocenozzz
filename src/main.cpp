#include <iostream>
#include <chrono>
#include <thread>
#include "world.hpp"
#include "logger.hpp"

int main(int argc, char** argv) {
    Logger logger;
    World world;

    PLOG_INFO << "plog called from main";

    return 0;
}
