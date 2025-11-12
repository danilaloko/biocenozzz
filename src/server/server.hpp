#pragma once

#include <plog/Log.h>
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <thread>
#include <atomic>
#include "core/world.hpp"

class Server {
private:
    httplib::Server svr;
    int port;
    std::thread server_thread;
    std::atomic<bool> running;
    World* world;

    void run_server();
    
    template<typename T>
    nlohmann::json serialize(const T* obj) const;

public:
    Server(int port, World* world);
    ~Server();
    
    void start();
    void stop();
};