#include <iostream>
#include "logger.hpp"

Logger::Logger(){
    plog::init(plog::debug, "logs/log.txt"); 
    plog::init<plog::TxtFormatter>(plog::debug, plog::streamStdOut); //init logger for console output either
    PLOG_DEBUG << "Logger initialised";
}