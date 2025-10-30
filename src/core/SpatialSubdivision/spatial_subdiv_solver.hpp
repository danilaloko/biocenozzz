#pragma once

#include <QObject>
#include "../world.hpp"

class SpatialSubdivSolver : public QObject 
{
    Q_OBJECT

public:
    SpatialSubdivSolver(World &world);

public slots:
    void pos_update_handler(QUuid, double x, double y);

private:
    World &world;
};