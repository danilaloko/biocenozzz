#pragma once 

#include <vector>
#include <QUuid>
#include <QObject>
#include "logger.hpp"
#include "species.hpp"

class Entity : public QObject {

    Q_OBJECT

public:
    QUuid id;
    Species* species; 
    int age;
    float energy;
    std::vector<Entity*> children;
    bool is_alive;
    double x;
    double y;

    Entity(Species* species_ptr);
    void update();
    void die();

    bool operator==(const Entity& other) const;

private:
    void _move();
    void _eat();
    void _reproduce();
    void _attack();
    void _sense();
};