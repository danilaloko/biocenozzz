// main.cpp
#include <QCoreApplication>
#include <QTimer>
#include <thread>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include <iostream>
#include "core/world.hpp"
#include "core/logger.hpp"
#include "core/entity.hpp"
#include "core/species.hpp"
#include "core/entity_factory.hpp"
#include "server/server.hpp"

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    
    std::srand(std::time(nullptr));
    
    Logger logger;
    World* world = new World(100, 100);
    
    Server server(8080, world);
    std::thread server_thread([&server]() {
        server.start();
    });
    
    EntityFactory factory(world);
    Rabbit rabbit_species;
    Grass grass_species;

    factory.create_entity(&rabbit_species, 10.0, 50.0);
    factory.create_entity(&rabbit_species, 90.0, 50.0);
    factory.create_entity(&rabbit_species, 50.0, 10.0);
    factory.create_entity(&rabbit_species, 50.0, 90.0);

    factory.create_entity(&grass_species, 30.0, 30.0);

    world->run();
    
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&]() {
        server.stop();
        if (server_thread.joinable()) {
            server_thread.join();
        }
    });
    
    return app.exec();
}