#pragma once
#include <QObject>
#include <QHash>
#include <vector>
#include <QUuid>
#include "entity.hpp"

class World : public QObject {
    Q_OBJECT

public:
    size_t size_x;
    size_t size_y;
    QHash<QUuid, Entity*> entity_map;

    World(size_t size_x, size_t size_y);
    void run();

public slots:
    void simulation_step();

private:
    QTimer* simulation_timer;
};