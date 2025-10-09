#include <crossguid/guid.hpp>
#include "flock.hpp"


Flock::Flock()
    : id(xg::newGuid())
{
    PLOG_DEBUG << "New flock created with uuid = {" << id << "}";
}
