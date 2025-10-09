#include <crossguid/guid.hpp>
#include "entity.hpp"

Entity::Entity() 
    : id(xg::newGuid())
{
    PLOG_DEBUG << "New entity created with uuid = {" << id << "}";
}