#pragma once

#include <QObject>
#include <vector>
#include <memory>
#include "../world.hpp"
#include "cell.hpp"

class SpatialSubdivSolver : public QObject {
    Q_OBJECT

public:
    double cell_size;
    World &world;
    std::vector<std::vector<std::unique_ptr<Cell>>> grid;
    QHash<QUuid, std::pair<int, int>> entity_cell_positions;

    SpatialSubdivSolver(World &world, double cell_size);
    
    Cell& get_cell(double x, double y);
    std::pair<int, int> get_cell_indices(double x, double y);
    std::vector<Entity*> get_collisions_by_id(QUuid entity_id);

public slots:
    void pos_update_handler(QUuid id, double x, double y);
};