#include "world.hpp"
#include <QTimer>
#include "species_worker.hpp"

World::World(size_t size_x, size_t size_y)
    : size_x(size_x), size_y(size_y)
{
}

World::~World() {
    stop();
}

void World::run() {
    PLOG_INFO << "Starting multi-threaded simulation";
    
    QHash<QString, std::vector<Entity*>> entities_by_type;
    
    for (auto it = entity_map.begin(); it != entity_map.end(); ++it) {
        Entity* entity = it.value();
        
        std::string type_name = entity->species->get_type().name();
        entities_by_type[QString::fromStdString(type_name)].push_back(entity);
    }
    
    PLOG_INFO << "Found " << entities_by_type.size() << " different species types";
    
    for (auto it = entities_by_type.begin(); it != entities_by_type.end(); ++it) {
        PLOG_DEBUG << "Type: " << it.key().toStdString() << " has " << it.value().size() << " entities";
    }
    
    for (auto it = entities_by_type.begin(); it != entities_by_type.end(); ++it) {
        QString type_name = it.key();
        std::vector<Entity*>& entities = it.value();
        
        SpeciesWorker* worker = new SpeciesWorker();
        for (Entity* entity : entities) {
            worker->addEntity(entity);
        }
        
        QThread* thread = new QThread();
        worker->moveToThread(thread);
        
        connect(thread, &QThread::started, worker, &SpeciesWorker::start);
        connect(thread, &QThread::finished, worker, &SpeciesWorker::deleteLater);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        
        thread->start();
        
        workers[type_name] = worker;
        threads[type_name] = thread;
        
        PLOG_DEBUG << "Started thread for type: " << type_name.toStdString() << " with " << entities.size() << " entities";
    }
    
    PLOG_INFO << "Total threads started: " << threads.size();
}

void World::stop() {
    PLOG_INFO << "Stopping " << threads.size() << " threads";
    for (QThread* thread : threads) {
        thread->quit();
        thread->wait();
    }
    workers.clear();
    threads.clear();
}