#pragma once
#include <QObject>
#include <QHash>
#include <vector>
#include <QUuid>
#include "entity.hpp"

class EntityManager : public QObject {
    Q_OBJECT

public:
    std::vector<Entity*> entities;
    QHash<QUuid, Entity*> entity_map;
};