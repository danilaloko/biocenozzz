#include <iostream>
#include <memory>
#include "world.hpp"
#include "logger.hpp"
#include "entity.hpp"
#include "flock.hpp"

int main(int argc, char** argv) {
    Logger logger;
    World world;

    // вот насчет shared_ptr не уверен...
    // с одной стороны, это смарт-указатель, но не надо пользоваться move семантикой (которую я не знаю)
    // с другой стороны, по смыслу больше подходит unique_ptr, ведь если брать shared, то объектом могут владеть несколько умных указателей, то есть entity может одновременно быть в нескольких стаях (вот этот момент надо обсудить)
    // короче, я просто хотел попрактиковаться в умных указателях...

    auto entity0 = std::make_shared<Entity>();
    auto entity1 = std::make_shared<Entity>();

    
    Flock flock0;
    Flock flock1;

    flock0 -= entity0;
    flock0 += entity0;
    flock1 += entity0; // TODO: проверка на наличие entity в стае. Если в стае уже есть - не добавляем
    flock0.leader = entity0; 

    flock1 += entity1;

    Flock flock2 = flock0 + flock1;
    return 0;
}