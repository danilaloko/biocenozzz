#pragma once

#include <QObject>
#include <QHash>
#include <vector>
#include <QUuid>
#include "../entity.hpp"

class Cell : public QObject {

Q_OBJECT

public:
    QHash<QUuid, Entity*> entity_map;

};