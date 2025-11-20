#pragma once
#include <QObject>
#include <QHash>
#include <vector>
#include <QUuid>
#include <QThread>
#include <QString>
#include "entity.hpp"

class SpeciesWorker;

class World : public QObject {
    Q_OBJECT

public:
    size_t size_x;
    size_t size_y;
    QHash<QUuid, Entity*> entity_map;

    World(size_t size_x, size_t size_y);
    ~World();
    void run();
    void stop();

    int getThreadCount() const { return threads.size(); }

private:
    QHash<QString, SpeciesWorker*> workers;
    QHash<QString, QThread*> threads;
};