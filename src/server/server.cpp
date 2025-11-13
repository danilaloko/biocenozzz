#include "server.hpp"

Server::Server(int port, World* world) 
    : port(port), running(false), world(world)
{
    PLOG_INFO << "Server created on port " << port;
}

Server::~Server() {
    stop();
}

void Server::start() {
    if (running) {
        PLOG_WARNING << "Server is already running";
        return;
    }

    running = true;
    run_server();
}

void Server::stop() {
    if (!running) return;

    running = false;
    svr.stop();
    PLOG_DEBUG << "Server stopped";
}

template<>
nlohmann::json Server::serialize<Entity>(const Entity* entity) const {
    nlohmann::json j;
    j["id"] = entity->id.toString().toStdString();
    j["x"] = entity->x;
    j["y"] = entity->y;
    
    if (dynamic_cast<const Rabbit*>(entity->species)) {
        j["species"] = "Rabbit";
    } else if (dynamic_cast<const Fox*>(entity->species)) {
        j["species"] = "Fox";
    } else {
        j["species"] = "Unknown";
    }
    
    j["age"] = entity->age;
    j["energy"] = entity->energy;
    j["is_alive"] = entity->is_alive;
    
    return j;
}

void Server::run_server() {
    svr.Get("/entities", [this](const httplib::Request&, httplib::Response& res) {
        try {
            nlohmann::json j = nlohmann::json::array();
            
            for (auto it = world->entity_map.begin(); it != world->entity_map.end(); ++it) {
                Entity* entity = it.value();
                j.push_back(serialize(entity));
            }
            
            res.set_content(j.dump(), "application/json");
        } catch (const std::exception& e) {
            PLOG_ERROR << "Error generating entities JSON: " << e.what();
            res.status = 500;
            res.set_content("{\"error\":\"Internal server error\"}", "application/json");
        }
    });

    PLOG_INFO << "Server listening on http://localhost:" << port;
    
    if (!svr.listen("localhost", port)) {
        PLOG_ERROR << "Failed to start server on localhost:" << port;
        running = false;
    }
}