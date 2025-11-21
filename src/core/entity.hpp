#pragma once 

#include <vector>
#include <QUuid>
#include <QObject>
#include <QReadWriteLock>
#include <atomic>
#include "logger.hpp"
#include "species.hpp"

enum class State {
    idle,
    searching_for_food
};

class Entity : public QObject {
    Q_OBJECT

public:
    QUuid id;
    Species* species; 
    std::atomic<int> age;
    std::atomic<float> energy;
    std::vector<Entity*> children;
    std::atomic<bool> is_alive;
    std::vector<Entity*> visible_entities;

    double _target_pos_x;
    double _target_pos_y;

    Entity(Species* species_ptr);
    void update();
    void die();

    bool operator==(const Entity& other) const;

    void update_pos(double target_x, double target_y);
    
    void initPosition(double x, double y);

    double getX() const;
    double getY() const;
    float getEnergy() const;
    bool getIsAlive() const;
    int getAge() const;

private:
    mutable QReadWriteLock position_lock;
    double x;
    double y;
    State _state;
    
    void _eat();
    void _reproduce();
    void _attack();
    void _sense();
    void _update_idle();
    void _update_searching_for_food();
    void _move_to_target();
    void _check_energy();
    Entity* _find_food();
    void _find_predators();
    bool _can_eat(Entity* other) const;
    bool _is_threat(Entity* other) const;

public slots:
    void on_other_entity_moved_ptr(Entity* other_entity);

signals:
    void entity_moved_signal(Entity* entity);
};