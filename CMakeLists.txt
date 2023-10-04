cmake_minimum_required(VERSION 3.25)
project(COMP345_RISK VERSION 1.0)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

add_executable(COMP345_RISK
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


