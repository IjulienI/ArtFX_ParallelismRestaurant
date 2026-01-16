
#include "Restaurant.h"
#include "spdlog/spdlog.h"

constexpr int CUSTOMER_COUNT = 5;

int main()
{
    spdlog::set_level(spdlog::level::info);
    spdlog::info("=== Restaurant Started ===");

    // Initialize Actors
    auto waiter = std::make_unique<Waiter>("Waiter_Bob");
    auto chef = std::make_unique<Chef>("Chef_Etchebest");
    auto cook1 = std::make_unique<Cook>("Cook_1");
    auto cook2 = std::make_unique<Cook>("Cook_2");

    waiter->setChef(chef.get());

    chef->setWaiter(waiter.get());
    chef->addCook(cook1.get());
    chef->addCook(cook2.get());

    cook1->setChef(chef.get());
    cook2->setChef(chef.get());

    waiter->start();
    chef->start();
    cook1->start();
    cook2->start();

    // Initialize Customers
    std::vector<std::unique_ptr<Customer>> customers;
    for (int i = 1; i <= CUSTOMER_COUNT; ++i)
    {
        auto customer = std::make_unique<Customer>(i, waiter.get());
        customer->start();
        customers.push_back(std::move(customer));

        // Wait
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Let the restaurant run for a while
    std::this_thread::sleep_for(std::chrono::seconds(8));

    spdlog::info("=== Restaurant Closing ===");
}
