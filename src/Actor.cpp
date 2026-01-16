#include "Actor.h"

#include <spdlog/sinks/stdout_color_sinks.h>

// Constructor initializes the actor's name and logger
Actor::Actor(std::string name) : name(std::move(name)), running(false)
{
    logger = spdlog::stdout_color_mt(this->name);
    logger->set_pattern("[%H:%M:%S] [%n] %^%v%$");
}

// Destructor ensures the actor is stopped
Actor::~Actor()
{
    stop();
}

// Start the actor's thread
void Actor::start()
{
    running = true;
    workerThread = std::thread(&Actor::run, this);
    logger->info(name + " started");
}

// Stop the actor's thread
void Actor::stop()
{
    if (running)
    {
        running = false;
        if (workerThread.joinable())
        {
            workerThread.detach();
        }
        logger->info(name + " stopped");
    }
}
