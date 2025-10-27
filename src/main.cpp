#include <QCoreApplication>
#include <iostream>
#include "world.hpp"
#include "logger.hpp"
#include "entity.hpp"
#include "species.hpp"
#include "flock.hpp"

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv); // Необходимо для работы Qt
    
    Logger logger;
    World world;

    Rabbit rabbit_species;
    Fox fox_species;

    // Создаем сущности с разными начальными позициями
    Entity* rabbit1 = new Entity(&rabbit_species);
    rabbit1->x = 0.0; rabbit1->y = 0.0;
    
    Entity* rabbit2 = new Entity(&rabbit_species);
    rabbit2->x = 1.0; rabbit2->y = 1.0;
    
    Entity* fox = new Entity(&fox_species);
    fox->x = 3.0; fox->y = 3.0;
    
    Flock<Rabbit> rabbit_flock;
    Flock<Fox> fox_flock;

    rabbit_flock += rabbit1;        
    rabbit_flock += rabbit2;       
    // rabbit_flock += fox;  // Это теперь вызовет ошибку, так как fox не Rabbit
    
    fox_flock += fox;
    // fox_flock += rabbit1; // И это тоже ошибка

    rabbit_flock.leader = rabbit1;
    fox_flock.leader = fox;

    // Запускаем несколько циклов обновления

    // Очистка памяти (в реальном приложении нужно аккуратно управлять памятью)
    delete rabbit1;
    delete rabbit2;
    delete fox;

    return 0;
}