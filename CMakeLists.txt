cmake_minimum_required(VERSION 3.25)
project(COMP345_RISK VERSION 1.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

# Toggle debug output macros.
option(DEBUG_MODE "Enables debugging code & macros." ON)

# Isolates code to run game engine, DEBUG_MODE must be on.
option(EXECUTE_CARDS              "Toggles execution of Cards code in debug mode."              OFF)
option(EXECUTE_GAME_ENGINE        "Toggles execution of GameEngine code in debug mode."         OFF)
option(EXECUTE_MAP                "Toggles execution of Map code in debug mode."                OFF)
option(EXECUTE_ORDERS             "Toggles execution of Orders code in debug mode."             OFF)
option(EXECUTE_PLAYER             "Toggles execution of Player code in debug mode."             OFF)
option(EXECUTE_COMMAND_PROCESSING "Toggles execution of Command Processing code in debug mode." ON)


if(DEBUG_MODE)
    add_definitions(-DDEBUG -DRED_DEBUG_OUTPUT)

    if(EXECUTE_CARDS)
        add_definitions(-DEXECUTE_CARDS)
    endif()
    if(EXECUTE_GAME_ENGINE)
        add_definitions(-DEXECUTE_GAME_ENGINE)
    endif()
    if(EXECUTE_MAP)
        add_definitions(-DEXECUTE_MAP)
    endif()
    if(EXECUTE_ORDERS)
        add_definitions(-DEXECUTE_ORDERS)
    endif()
    if(EXECUTE_PLAYER)
        add_definitions(-DEXECUTE_PLAYER)
    endif()
    if(EXECUTE_COMMAND_PROCESSING)
        add_definitions(-DEXECUTE_COMMAND_PROCESSING)
    endif()
endif()


add_executable(COMP345_RISK
        # .h files
        headers/Cards.h headers/GameEngine.h headers/Map.h headers/Orders.h headers/Player.h
        headers/CommandProcessing.h

        # .cpp files
        src/Cards.cpp src/CardsDriver.cpp src/GameEngine.cpp src/GameEngineDriver.cpp src/Map.cpp src/MapDriver.cpp
        src/Orders.cpp src/OrdersDriver.cpp src/Player.cpp src/PlayerDriver.cpp src/MainDriver.cpp
        src/CommandProcessing.cpp src/CommandProcessingDriver.cpp)


