#pragma once 

#include <vector>
#include <QUuid>
#include <QObject>
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
    int age;
    float energy;
    std::vector<Entity*> children;
    bool is_alive;
    double x;
    double y;
    std::vector<Entity*> visible_entities;

    double _target_pos_x;
    double _target_pos_y;

    Entity(Species* species_ptr);
    void update();
    void die();

    bool operator==(const Entity& other) const;

    void update_pos(double target_x, double target_y);

private:
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
    void on_other_entity_moved(QUuid other_id, double other_x, double other_y);
    void on_other_entity_moved_ptr(Entity* other_entity);

signals:
    signals:
    void entity_moved_signal(Entity* entity);
    void update_pos_signal(QUuid id, double x, double y);
};