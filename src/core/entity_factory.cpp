#include "entity_factory.hpp"

EntityFactory::EntityFactory(World* world_ptr) 
    : world(world_ptr) 
{
}

Entity* EntityFactory::create_entity(Species* species_ptr, double x, double y) {
    Entity* new_entity = new Entity(species_ptr);
    new_entity->x = x;
    new_entity->y = y;
    new_entity->_target_pos_x = x;
    new_entity->_target_pos_y = y;
    
    world->entity_map.insert(new_entity->id, new_entity);
    
    for (auto it = world->entity_map.begin(); it != world->entity_map.end(); ++it) {
        Entity* existing_entity = it.value();
        
        if (existing_entity->id == new_entity->id) {
            continue;
        }
        
        connect(existing_entity, &Entity::entity_moved_signal,
                new_entity, &Entity::on_other_entity_moved_ptr);
        
        connect(new_entity, &Entity::entity_moved_signal,
                existing_entity, &Entity::on_other_entity_moved_ptr);
    }
    
    PLOG_DEBUG << "Entity created and connected: " << new_entity->id.toString().toStdString()
               << " at (" << x << ", " << y << ")";
    return new_entity;
}