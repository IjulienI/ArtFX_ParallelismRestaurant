#include "Restaurant.h"
#include <random>
#include <utility>

// Cook
// ------------------------------------------------------------------
Cook::Cook(std::string name) : Actor(std::move(name))
{
}

void Cook::setChef(Chef* chef)
{
    chefRef = chef;
}

void Cook::addTask(CookingTask task)
{
    inbox.push(std::move(task));
}

void Cook::run()
{
    CookingTask task;
    while (running)
    {
        if (inbox.pop(task))
        {
            logger->info("Cooking of : {} for Order ID: {} started.", task.ingredient, task.orderId);
            std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand()%500));

            task.ingredient = "Cooked " + task.ingredient;
            if (chefRef)
            {
                chefRef->receiveCookedIngredient(task);
            }
        }
    }
}

// Chef
// ------------------------------------------------------------------
Chef::Chef(std::string name) : Actor(std::move(name))
{
}

void Chef::addCook(Cook* cook)
{
    cooks.push_back(cook);
}

void Chef::setWaiter(Waiter* waiter)
{
    waiterRef = waiter;
}

void Chef::receiveOrder(Order order)
{
    orderInbox.push(std::move(order));
}

void Chef::receiveCookedIngredient(CookingTask task)
{
    cookedInbox.push(std::move(task));
}

void Chef::run()
{
    while (running)
    {
        if (!orderInbox.empty())
        {
            Order order;
            if (orderInbox.pop(order))
            {
                logger->info("Command #{} received (Client {}. Gived to {} Cook)", order.id, order.customerId, cooks.size());
                ordersInProgress[order.id] = order;

                for (const auto& ingredient : order.ingredients)
                {
                    if (!cooks.empty())
                    {
                        cooks[rand() % cooks.size()]->addTask({order.id, ingredient});
                    }
                }
            }
        }

        if (!cookedInbox.empty())
        {
            CookingTask task;
            if (cookedInbox.pop(task))
            {
                auto& order = ordersInProgress[task.orderId];
                order.cookedIngredients.push_back(task.ingredient);

                if (order.cookedIngredients.size() == 3)
                {
                    logger->warn("Command #{} is ready to serve!", order.id);
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));

                    order.isReady = true;
                    if (waiterRef)
                    {
                        waiterRef->serveMeal(order);
                        ordersInProgress.erase(task.orderId);
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// Waiter
// ------------------------------------------------------------------
Waiter::Waiter(std::string name) : Actor(std::move(name))
{
}

void Waiter::setChef(Chef* chef)
{
    chefRef = chef;
}

void Waiter::takeOrder(Order order)
{
    toKitchenQueue.push(std::move(order));
}

void Waiter::serveMeal(Order order)
{
    toServeQueue.push(std::move(order));
}

void Waiter::run()
{
    while (running)
    {
        if (!toServeQueue.empty())
        {
            Order order;
            toServeQueue.pop(order);
            logger->critical("Order #{} served to Customer {}!", order.id, order.customerId);
        }
        else if (!toKitchenQueue.empty())
        {
            Order order;
            toKitchenQueue.pop(order);
            logger->info("Order #{} taken from Customer {} and sent to Kitchen.", order.id, order.customerId);
            if (chefRef)
            {
                chefRef->receiveOrder(order);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

// Customer
// ------------------------------------------------------------------
Customer::Customer(int id, Waiter* waiter) : Actor("Customer_" + std::to_string(id)), id(id), waiterRef(waiter)
{
}

void Customer::run()
{
    std::random_device rd;
    std::mt19937 generator(rd());

    std::uniform_int_distribution<> sleepDist(200, 1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepDist(generator)));

    Order order;
    order.id = ++globalOrderCounter;
    order.customerId = id;

    std::vector<std::string> menu = {"Steak", "Salad", "Soup", "Pasta", "Fish"};
    std::uniform_int_distribution<> menuDist(0, static_cast<int>(menu.size()) - 1);

    for (int i = 0; i < 3; ++i)
    {
        order.ingredients.push_back(menu[menuDist(generator)]);
    }

    logger->info("I would like to order : {}, {}, {}.", order.ingredients[0], order.ingredients[1], order.ingredients[2]);
    waiterRef->takeOrder(order);
}

