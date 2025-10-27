#pragma once
#include <QObject>
#include <vector>
#include <unordered_map>
#include <QUuid>
#include "entity.hpp"

class EntityManager : public QObject {
    Q_OBJECT
public:
    std::vector<Entity*> entities;
    std::unordered_map<QUuid, Entity*> entity_map;
};