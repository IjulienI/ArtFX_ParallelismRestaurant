#pragma once
#include <map>

#include "Actor.h"
#include "Common.h"
#include "SafeQueue.h"

class Chef;
class Waiter;

// Cook
// ------------------------------------------------------------------
class Cook :  public Actor
{
    SafeQueue<CookingTask> inbox;
    Chef* chefRef {nullptr};

public:
    explicit Cook(std::string name);
    void setChef(Chef* chef);
    void addTask(CookingTask task);

protected:
    void run() override;
};

// Chef
// ------------------------------------------------------------------
class Chef : public Actor
{
    SafeQueue<Order> orderInbox;
    SafeQueue<CookingTask> cookedInbox;
    std::vector<Cook*> cooks;
    Waiter* waiterRef {nullptr};
    std::map<int, Order> ordersInProgress;

public:
    explicit Chef(std::string name);
    void addCook(Cook* cook);
    void setWaiter(Waiter* waiter);
    void receiveOrder(Order order);
    void receiveCookedIngredient(CookingTask task);

protected:
    void run() override;
};

// Waiter
// ------------------------------------------------------------------
class Waiter : public Actor
{
    SafeQueue<Order> toServeQueue;
    SafeQueue<Order> toKitchenQueue;
    Chef* chefRef {nullptr};

public:
    explicit Waiter(std::string name);
    void setChef(Chef* chef);
    void takeOrder(Order order);
    void serveMeal(Order order);

protected:
    void run() override;
};

// Customer
// ------------------------------------------------------------------
class Customer : public Actor
{
    Waiter* waiterRef;
    int id;

public:
    Customer(int id, Waiter* waiter);

protected:
    void run() override;
};