#include <QUuid>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "entity.hpp"

Entity::Entity(Species* species_ptr) 
    : QObject(), id(QUuid::createUuid()), species(species_ptr), age(0), 
      energy(species_ptr->max_energy), is_alive(true), _target_pos_x(0.0), _target_pos_y(0.0),
      x(0.0), y(0.0), _state(State::idle)
{
    visible_entities.clear();
    _target_pos_x = x;
    _target_pos_y = y;
    
    PLOG_DEBUG << "New entity created with uuid = {" << id.toString().toStdString() 
               << "} at position (" << x << ", " << y << ") with energy: " << energy.load();
}

void Entity::initPosition(double x, double y) {
    {
        QWriteLocker locker(&position_lock);
        this->x = x;
        this->y = y;
    }
    _target_pos_x = x;
    _target_pos_y = y;
}

double Entity::getX() const {
    QReadLocker locker(&position_lock);
    return x;
}

double Entity::getY() const {
    QReadLocker locker(&position_lock);
    return y;
}

float Entity::getEnergy() const {
    return energy.load(std::memory_order_acquire);
}

bool Entity::getIsAlive() const {
    return is_alive.load(std::memory_order_acquire);
}

int Entity::getAge() const {
    return age.load(std::memory_order_acquire);
}

void Entity::update() {
    if (species->category != TrophicCategory::Producer) {
        float current_energy = energy.load(std::memory_order_acquire);
        energy.store(current_energy - 0.01f, std::memory_order_release);
        
        if (current_energy - 0.01f <= 0.0f) {
            die();
            return;
        }
    }
    
    _check_energy();
    
    switch (_state) {
        case State::idle:
            _update_idle();
            break;
            
        case State::searching_for_food:
            _update_searching_for_food();
            break;
    }
    
    _move_to_target();

    if (species->speed == 0.0f) {
        emit entity_moved_signal(this);
    }
}

void Entity::_check_energy() {
    float hunger_threshold = species->max_energy * 0.75f;
    float current_energy = getEnergy();
    
    if (current_energy <= hunger_threshold && _state != State::searching_for_food) {
        _state = State::searching_for_food;
        PLOG_DEBUG << "Entity " << id.toString().toStdString() 
                   << " is now hungry (energy: " << current_energy << ") and starts searching for food";
    } else if (current_energy > hunger_threshold && _state == State::searching_for_food) {
        _state = State::idle;
        PLOG_DEBUG << "Entity " << id.toString().toStdString() 
                   << " is no longer hungry (energy: " << current_energy << ") and returns to idle";
    }
}

Entity* Entity::_find_food() {
    for (Entity* other : visible_entities) {
        if (_can_eat(other)) {
            return other;
        }
    }
    return nullptr;
}

void Entity::_find_predators() {
}

void Entity::_update_searching_for_food() {
    Entity* food_target = _find_food();
    
    if (food_target) {
        _target_pos_x = food_target->getX();
        _target_pos_y = food_target->getY();
        
        PLOG_DEBUG << "Entity " << id.toString().toStdString() 
                   << " moving towards food at (" << _target_pos_x << ", " << _target_pos_y << ")";
    } else {
        double current_x = getX();
        double current_y = getY();
        double angle = (std::rand() % 360) * 3.14 / 180.0;
        double distance = species->sense_radius;
        
        _target_pos_x = current_x + std::cos(angle) * distance;
        _target_pos_y = current_y + std::sin(angle) * distance;
        
        _target_pos_x = std::max(0.0, std::min(_target_pos_x, 100.0));
        _target_pos_y = std::max(0.0, std::min(_target_pos_y, 100.0));
        
        PLOG_DEBUG << "Entity " << id.toString().toStdString() 
                   << " no food found, exploring to (" << _target_pos_x << ", " << _target_pos_y << ")";
    }
}

bool Entity::_can_eat(Entity* other) const {
    auto it = std::find(species->diet.begin(), species->diet.end(), 
                       std::type_index(typeid(*(other->species))));
    return it != species->diet.end();
}

bool Entity::_is_threat(Entity* other) const {
    auto it = std::find(other->species->diet.begin(), other->species->diet.end(), 
                       std::type_index(typeid(*(this->species))));
    return it != other->species->diet.end();
}

void Entity::update_pos(double target_x, double target_y) {
    {
        QWriteLocker locker(&position_lock);
        x = target_x;
        y = target_y;
    }
    emit entity_moved_signal(this);
}

void Entity::on_other_entity_moved_ptr(Entity* other_entity) {
    if (other_entity->id == this->id) {
        return;
    }

    double this_x = getX();
    double this_y = getY();
    double other_x = other_entity->getX();
    double other_y = other_entity->getY();
    
    double distance = std::sqrt(std::pow(this_x - other_x, 2) + 
                              std::pow(this_y - other_y, 2));
    
    if (distance <= species->sense_radius) {
        if (std::find(visible_entities.begin(), visible_entities.end(), other_entity) == visible_entities.end()) {
            visible_entities.push_back(other_entity);
        }
    } else {
        auto it = std::find(visible_entities.begin(), visible_entities.end(), other_entity);
        if (it != visible_entities.end()) {
            visible_entities.erase(it);
        }
    }
}

void Entity::die() {
    is_alive.store(false, std::memory_order_release);
    PLOG_DEBUG << "Entity " << id.toString().toStdString() << " died.";
}

void Entity::_update_idle() {
    double current_x = getX();
    double current_y = getY();
    double distance_to_target = std::sqrt(std::pow(current_x - _target_pos_x, 2) + 
                                        std::pow(current_y - _target_pos_y, 2));
    
    if (distance_to_target < 1.0) {
        double angle = (std::rand() % 360) * 3.14 / 180.0;
        double distance = (std::rand() % static_cast<int>(species->sense_radius));
        
        _target_pos_x = current_x + std::cos(angle) * distance;
        _target_pos_y = current_y + std::sin(angle) * distance;
        
        _target_pos_x = std::max(0.0, std::min(_target_pos_x, static_cast<double>(100)));
        _target_pos_y = std::max(0.0, std::min(_target_pos_y, static_cast<double>(100)));
    }
}

void Entity::_move_to_target() {
    double current_x = getX();
    double current_y = getY();
    double direction_x = _target_pos_x - current_x;
    double direction_y = _target_pos_y - current_y;
    double length = std::sqrt(direction_x * direction_x + direction_y * direction_y);
    
    if (length < 0.5) {
        return;
    }

    direction_x /= length;
    direction_y /= length;
    
    double new_x = current_x + direction_x * species->speed * 0.1;
    double new_y = current_y + direction_y * species->speed * 0.1;
    
    update_pos(new_x, new_y);
}

void Entity::_eat() {
}

void Entity::_reproduce() {
}

void Entity::_attack() {
}

void Entity::_sense() {
}

bool Entity::operator==(const Entity& other) const {
    return id == other.id;
}