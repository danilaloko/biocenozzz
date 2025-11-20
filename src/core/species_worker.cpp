#include "species_worker.hpp"
#include "core/entity.hpp"

SpeciesWorker::SpeciesWorker()
    : timer(new QTimer(this))
{
    connect(timer, &QTimer::timeout, this, &SpeciesWorker::processEntities);
}

void SpeciesWorker::addEntity(Entity* entity) {
    entities.push_back(entity);
}

void SpeciesWorker::start() {
    PLOG_DEBUG << "SpeciesWorker started in thread: " << QThread::currentThreadId();
    timer->start(100); // 10 FPS
}

void SpeciesWorker::stop() {
    timer->stop();
    PLOG_DEBUG << "SpeciesWorker stopped in thread: " << QThread::currentThreadId();
}

void SpeciesWorker::processEntities() {
    for (Entity* entity : entities) {
        if (entity->is_alive) {
            entity->update();
        }
    }
}