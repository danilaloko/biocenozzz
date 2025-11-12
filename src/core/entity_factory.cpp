// entity_factory.cpp
#include "entity_factory.hpp"

EntityFactory::EntityFactory(World* world_ptr) 
    : world(world_ptr) 
{
}

Entity* EntityFactory::create_entity(Species* species_ptr, double x, double y) {
    Entity* new_entity = new Entity(species_ptr);
    new_entity->x = x;
    new_entity->y = y;
    
    world->entity_map.insert(new_entity->id, new_entity);
    
    for (auto it = world->entity_map.begin(); it != world->entity_map.end(); ++it) {
        Entity* existing_entity = it.value();
        
        if (existing_entity->id == new_entity->id) {
            continue;
        }
        
        connect(existing_entity, &Entity::update_pos_signal,
                new_entity, &Entity::on_other_entity_moved);
        
        connect(new_entity, &Entity::update_pos_signal,
                existing_entity, &Entity::on_other_entity_moved);
    }
    
    PLOG_DEBUG << "Entity created and connected: " << new_entity->id.toString().toStdString();
    return new_entity;
}