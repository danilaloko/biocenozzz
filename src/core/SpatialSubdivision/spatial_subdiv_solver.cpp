#include "spatial_subdiv_solver.hpp"
#include <QDebug>
#include <iostream>
#include <cmath>

SpatialSubdivSolver::SpatialSubdivSolver(World &world, double cell_size)
    : QObject(), world(world), cell_size(cell_size) {
    
    int cells_x = std::ceil(world.size_x / cell_size);
    int cells_y = std::ceil(world.size_y / cell_size);
    
    grid.resize(cells_x);
    for (int i = 0; i < cells_x; ++i) {
        grid[i].reserve(cells_y);
        for (int j = 0; j < cells_y; ++j) {
            grid[i].push_back(std::make_unique<Cell>(cell_size));
        }
    }
    
    PLOG_DEBUG << "Spatial subdivision solver initialized. Grid: " 
               << cells_x << "x" << cells_y << " cells";
}

Cell& SpatialSubdivSolver::get_cell(double x, double y) {
    auto indices = get_cell_indices(x, y);
    return *grid[indices.first][indices.second];
}

std::pair<int, int> SpatialSubdivSolver::get_cell_indices(double x, double y) {
    int cell_x = std::floor(x / cell_size);
    int cell_y = std::floor(y / cell_size);
    
    cell_x = std::max(0, std::min(cell_x, (int)grid.size() - 1));
    cell_y = std::max(0, std::min(cell_y, (int)grid[0].size() - 1));
    
    return {cell_x, cell_y};
}

std::vector<Entity*> SpatialSubdivSolver::get_collisions_by_id(QUuid entity_id) {
    Entity* entity = world.entity_map.value(entity_id);
    if (!entity) {
        PLOG_DEBUG << "Entity not found for collision check: " << entity_id.toString().toStdString();
        return {};
    }
    
    double radius = entity->species->sense_radius;
    auto center_cell = get_cell_indices(entity->getX(), entity->getY());
    std::vector<Entity*> result;
    
    PLOG_DEBUG << "Checking collisions for entity " << entity_id.toString().toStdString()
               << " at (" << entity->getX() << ", " << entity->getY() << ") with radius " << radius;
    
   
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int check_x = center_cell.first + dx;
            int check_y = center_cell.second + dy;
            

            if (check_x >= 0 && check_x < grid.size() && 
                check_y >= 0 && check_y < grid[0].size()) {
                
                Cell& cell = *grid[check_x][check_y];
                PLOG_DEBUG << "Checking cell (" << check_x << ", " << check_y 
                           << ") with " << cell.entity_map.size() << " entities";
                
                for (auto it = cell.entity_map.begin(); it != cell.entity_map.end(); ++it) {
                    Entity* other = it.value();
                    
                    if (other->id == entity_id) continue;
                    

                    double distance_sq = std::pow(entity->getX() - other->getX(), 2) + 
                                       std::pow(entity->getY() - other->getY(), 2);
                    double distance = std::sqrt(distance_sq);
                    
                    PLOG_DEBUG << "Distance to entity " << other->id.toString().toStdString()
                               << " at (" << other->getX() << ", " << other->getY() << "): " << distance;
                    
                    if (distance_sq <= radius * radius) {
                        result.push_back(other);
                        PLOG_DEBUG << "Entity " << entity_id.toString().toStdString()
                                   << " COLLISION with " << other->id.toString().toStdString()
                                   << " at distance " << distance;
                    }
                }
            }
        }
    }
    
    PLOG_DEBUG << "Found " << result.size() << " collisions for entity " 
               << entity_id.toString().toStdString();
    return result;
}

void SpatialSubdivSolver::pos_update_handler(QUuid id, double x, double y) {
    Entity* entity = world.entity_map.value(id);
    if (!entity) {
        PLOG_ERROR << "Entity with ID " << id.toString().toStdString() << " not found in world";
        return;
    }

    auto new_cell_indices = get_cell_indices(x, y);
    

    if (entity_cell_positions.contains(id)) {
        auto old_cell_indices = entity_cell_positions[id];
        
        if (old_cell_indices != new_cell_indices) {

            grid[old_cell_indices.first][old_cell_indices.second]->remove_entity(id);
            

            grid[new_cell_indices.first][new_cell_indices.second]->add_entity(entity);
            entity_cell_positions[id] = new_cell_indices;
            
            PLOG_DEBUG << "Entity " << id.toString().toStdString() 
                      << " moved from cell (" << old_cell_indices.first << "," << old_cell_indices.second 
                      << ") to (" << new_cell_indices.first << "," << new_cell_indices.second << ")";
        }
    } else {

        grid[new_cell_indices.first][new_cell_indices.second]->add_entity(entity);
        entity_cell_positions[id] = new_cell_indices;
        
        PLOG_DEBUG << "Entity " << id.toString().toStdString() 
                  << " added to cell (" << new_cell_indices.first << "," << new_cell_indices.second << ")";
    }
    
    auto nearby_entities = get_collisions_by_id(id);
    
    entity->visible_entities.clear();
    for (Entity* other : nearby_entities) {
        entity->visible_entities.push_back(other);
        
        double distance = std::sqrt(
            std::pow(entity->getX() - other->getX(), 2) + 
            std::pow(entity->getY() - other->getY(), 2)
        );
        
        PLOG_DEBUG << "Entity " << id.toString().toStdString() 
                  << " detected entity " << other->id.toString().toStdString()
                  << " at distance " << distance;
    }
}