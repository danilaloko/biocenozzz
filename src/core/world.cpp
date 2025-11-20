#include "world.hpp"
#include <QTimer>

World::World(size_t size_x, size_t size_y)
    : size_x(size_x), size_y(size_y), simulation_timer(nullptr)
{
    simulation_timer = new QTimer(this);
    connect(simulation_timer, &QTimer::timeout, this, &World::simulation_step);
}

void World::run() {
    PLOG_INFO << "Starting simulation loop";
    simulation_timer->start(100);
}

void World::simulation_step() {
    for (auto it = entity_map.begin(); it != entity_map.end(); ++it) {
        Entity* entity = it.value();
        if (entity->is_alive) {
            entity->update();
        }
    }
}