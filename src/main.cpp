// main.cpp
#include <QCoreApplication>
#include <QTimer>
#include <thread>
#include <cmath>

#include <iostream>
#include "core/world.hpp"
#include "core/logger.hpp"
#include "core/entity.hpp"
#include "core/species.hpp"
#include "core/entity_factory.hpp"
#include "server/server.hpp"


int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    
    Logger logger;
    World* world = new World(100, 100);
    
    Server server(8080, world);
    std::thread server_thread([&server]() {
        server.start();
    });
    
    EntityFactory factory(world);
    Rabbit rabbit_species;

    Entity* rabbit1 = factory.create_entity(&rabbit_species, 10.0, 50.0);
    Entity* rabbit2 = factory.create_entity(&rabbit_species, 90.0, 50.0);
    
    double pos1 = 10.0;
    double pos2 = 90.0;
    double speed1 = 0.5;
    double speed2 = -0.5;
    
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        pos1 += speed1;
        pos2 += speed2;
        
        if (pos1 <= 10.0 || pos1 >= 90.0) {
            speed1 = -speed1;
        }
        
        if (pos2 <= 10.0 || pos2 >= 90.0) {
            speed2 = -speed2;
        }
        
        rabbit1->update_pos(pos1, 50.0);
        rabbit2->update_pos(pos2, 50.0);
        
    });
    
    timer.start(10);
    
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&]() {
        server.stop();
        if (server_thread.joinable()) {
            server_thread.join();
        }
    });
    
    return app.exec();
}