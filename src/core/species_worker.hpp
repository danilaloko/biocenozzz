#pragma once

#include <QObject>
#include <QThread>
#include <QTimer>
#include <vector>

class Entity;

class SpeciesWorker : public QObject {
    Q_OBJECT

public:
    SpeciesWorker();
    void addEntity(Entity* entity);

public slots:
    void start();
    void stop();
    void processEntities();

private:
    std::vector<Entity*> entities;
    QTimer* timer;
};