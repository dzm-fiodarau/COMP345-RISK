cmake_minimum_required(VERSION 3.25)
project(COMP345_RISK VERSION 1.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

# Toggle debug output macros
# add_definitions(-DDEBUG -DRED_DEBUG_OUTPUT)


add_executable(COMP345_RISK
        # .h files
        headers/Cards.h
        headers/GameEngine.h
        headers/Map.h
        headers/Orders.h
        headers/Player.h

        # .cpp files
        src/Cards.cpp
        src/CardsDriver.cpp
        src/GameEngine.cpp
        src/GameEngineDriver.cpp
        src/Map.cpp
        src/MapDriver.cpp
        src/Orders.cpp
        src/OrdersDriver.cpp
        src/Player.cpp
        src/PlayerDriver.cpp
        src/MainDriver.cpp)


