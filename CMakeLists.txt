cmake_minimum_required(VERSION 3.20)
project(ParallelismRestaurant)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(spdlog)

include_directories(include)
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.h")
add_executable(ParallelismRestaurant ${SRC_FILES} ${HEADER_FILES})

target_link_libraries(ParallelismRestaurant PRIVATE spdlog::spdlog)
if(UNIX)
    target_link_libraries(ParallelismRestaurant pthread)
endif()
