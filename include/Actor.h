#pragma once

#include <memory>
#include <thread>
#include <spdlog/spdlog.h>

class Actor
{
protected:
    std::thread workerThread;
    std::atomic<bool> running;
    std::string name;
    std::shared_ptr<spdlog::logger> logger;

    virtual void run() = 0;

public:
    explicit Actor(std::string name);
    virtual ~Actor();

    void start();
    void stop();
    [[nodiscard]] std::string getName() const { return name; }
};
