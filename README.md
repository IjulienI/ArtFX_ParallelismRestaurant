# ArtFX_ParallelismRestaurant

A Modern C++20 implementation of the **Actor Model** simulating a high-concurrency restaurant environment.

## Features
- **Actor Model Architecture:** Each entity runs in its own thread and communicates exclusively via message passing.
- **Thread Safety:** Complete absence of race conditions using `std::mutex`, `std::condition_variable`, and `std::atomic`.
- **Modern C++17:** Usage of smart pointers `std::unique_ptr`, `std::shared_ptr`, lambdas, and modern random generation `std::mt19937`.
- **Structured Logging:** Integration of [spdlog](https://github.com/gabime/spdlog) for thread-safe, colored, and timestamped console output.
- **Clean Code:** Strict separation of declarations `.h` and implementations `.cpp` with a generic `Actor` base class.

## Architecture
**1. Actor Model**
The core of the system is the abstract `Actor` class.
- **Lifecyle:** Every actor manages its own `std::thread`.
- **Loop:** They execute a continuous `run()` loop until stopped.
- **Communication:** Actors do not call each other's functions directly. Instead, they push data (`Orders` or `Tasks`) into `SafeQueues`.

**2. Workflow**
  
