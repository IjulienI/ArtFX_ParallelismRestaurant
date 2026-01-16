#pragma once

#include <string>
#include <vector>
#include <atomic>

static std::atomic<int> globalOrderCounter{1};

struct Order
{
    int id {0};
    int customerId {0};
    std::vector<std::string> ingredients;
    std::vector<std::string> cookedIngredients;
    bool isReady {false};
};

struct CookingTask
{
    int orderId;
    std::string ingredient;
};
