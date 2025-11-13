// main.cpp
#include <QCoreApplication>
#include <QTimer>

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
    server.start();

    EntityFactory factory(world);

    Rabbit rabbit_species;

    Entity* rabbit1 = factory.create_entity(&rabbit_species, 2.0, 2.0);
    Entity* rabbit2 = factory.create_entity(&rabbit_species, 25.0, 25.0);
    
    PLOG_DEBUG << "Moving rabbit1 closer to rabbit2...";
    rabbit1->update_pos(23.0, 23.0);
    
    PLOG_DEBUG << "Moving rabbit2 even closer...";
    rabbit2->update_pos(24.0, 24.0);

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&server]() {
        server.stop();
    });
    
    return app.exec();
}