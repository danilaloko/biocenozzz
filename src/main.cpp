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
    
    PLOG_INFO << "Creating entities...";
    
    Grass grass_species;
    Rabbit rabbit_species; 
    Fox fox_species;
    
    factory.create_entity(&grass_species, 30.0, 30.0);
    factory.create_entity(&grass_species, 70.0, 70.0);
    factory.create_entity(&rabbit_species, 10.0, 50.0);
    factory.create_entity(&rabbit_species, 90.0, 50.0);
    factory.create_entity(&fox_species, 20.0, 40.0);

    PLOG_INFO << "Total entities created: " << world->entity_map.size();
    
    world->run();
    
    PLOG_INFO << "Simulation running with " << world->getThreadCount() << " threads";
    
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&]() {
        PLOG_INFO << "Shutting down simulation...";
        world->stop();
        server.stop();
        if (server_thread.joinable()) {
            server_thread.join();
        }
        delete world;
        PLOG_INFO << "Simulation stopped";
    });
    
    return app.exec();
}