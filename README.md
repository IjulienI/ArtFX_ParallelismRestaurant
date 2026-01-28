# ArtFX_ParallelismRestaurant

A Modern C++20 implementation of the **Actor Model** simulating a high-concurrency restaurant environment.

## Features
- **Actor Model Architecture:** Each entity runs in its own thread and communicates exclusively via message passing.
- **Thread Safety:** Complete absence of race conditions using `std::mutex`, `std::condition_variable`, and `std::atomic`.
- **Modern C++20:** Usage of smart pointers `std::unique_ptr`, `std::shared_ptr`, lambdas, and modern random generation `std::mt19937`.
- **Structured Logging:** Integration of [spdlog](https://github.com/gabime/spdlog) for thread-safe, colored, and timestamped console output.
- **Clean Code:** Strict separation of declarations `.h` and implementations `.cpp` with a generic `Actor` base class.

## Architecture
**1. Actor Model**
The core of the system is the abstract `Actor` class.
- **Lifecyle:** Every actor manages its own `std::thread`.
- **Loop:** They execute a continuous `run()` loop until stopped.
- **Communication:** Actors do not call each other's functions directly. Instead, they push data (`Orders` or `Tasks`) into `SafeQueues`.

**2. Workflow**
![mermaid-drawing](https://github.com/user-attachments/assets/126ab026-ece8-405a-9d20-d0a51f5cb4a4)

**3. Roles**
 	| Class | Role and Responsability |
| ----------- | ----------- |
| `Actor` | Base class. Manages the thread, the `running` state, and the logger. |
| `SafeQueue<T>` | A thread-safe FIFO queue using mutexes and condition variables for blocking `pop()`. | 
| `Customer` | Generates a unique order with a random menu using `std::mt19937` and sends it to the Waiter. |
| `Waiter` | The middle-man. Takes orders to the kitchen and serves ready meals back to customers. | 
| `Chef` | The orchestrator. Decomposes orders into individual tasks, load-balances them to Cooks, and re-assembles the meal once ingredients are cooked. |
| `Cook` | A worker. Receives a raw ingredient, sleeps, and returns it cooked. | 

## Build and Run
**Prerequisites**
- C++ Compiler supporting C++20.
- CMake 3.20+.

**Instructions**
1. **Clone the repository**
   ``` bash
   git clone https://github.com/IjulienI/ArtFX_ParallelismRestaurant.git
   cd ArtFX_ParallelismRestaurant
   ```
2. **Build with CMake**
   ``` bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
3.  **Run the program**
    ``` bash
    ./Debug/ParallelismRestaurant
    ```
