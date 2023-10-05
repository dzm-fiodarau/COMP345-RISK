cmake_minimum_required(VERSION 3.25)
project(COMP345_RISK VERSION 1.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

# Debug Mode, delete before commit
add_definitions(-DDEBUG
        -DRED_DEBUG_OUTPUT)

add_executable(GAME_ENGINE_DEBUG
        headers/GameEngine.h
        src/GameEngine.cpp
        src/GameEngineDriver.cpp)

add_executable(COMP345_RISK
        # .h files
        headers/Cards.h
        headers/Map.h
        headers/Orders.h
        headers/Player.h

        # .cpp files
        src/Cards.cpp
        src/CardsDriver.cpp
        src/GameEngineDriver.cpp
        src/Map.cpp
        src/MapDriver.cpp
        src/Orders.cpp
        src/OrdersDriver.cpp
        src/Player.cpp
        src/PlayerDriver.cpp
        src/MainDriver.cpp)


