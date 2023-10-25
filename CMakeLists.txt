include(FetchContent)

cmake_minimum_required(VERSION 3.25)
project(COMP345_RISK VERSION 1.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


#   FETCHING DEPENDENCIES/LIBRARIES
# googletest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)



#   BUILD OPTIONS
# Toggle debug output macros.
option(DEBUG_MODE "Enables debugging code & macros." OFF)

# Isolates code to run game engine, DEBUG_MODE must be on.
option(EXECUTE_CARDS              "Toggles execution of Cards code in debug mode."              OFF)
option(EXECUTE_GAME_ENGINE        "Toggles execution of GameEngine code in debug mode."         OFF)
option(EXECUTE_MAP                "Toggles execution of Map code in debug mode."                OFF)
option(EXECUTE_ORDERS             "Toggles execution of Orders code in debug mode."             OFF)
option(EXECUTE_PLAYER             "Toggles execution of Player code in debug mode."             OFF)
option(EXECUTE_COMMAND_PROCESSING "Toggles execution of Command Processing code in debug mode." OFF)



#   PROCESSING OPTIONS
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



#   BUILDING FILES
# Defining the variables for the source files
set(HEADERS
        headers/Cards.h headers/GameEngine.h headers/Map.h headers/Orders.h headers/Player.h headers/CommandProcessing.h)

set(SOURCES
        src/Cards.cpp src/GameEngine.cpp src/Map.cpp src/Orders.cpp src/Player.cpp src/CommandProcessing.cpp)

set(DRIVERS
        src/CardsDriver.cpp src/GameEngineDriver.cpp src/MapDriver.cpp src/OrdersDriver.cpp src/PlayerDriver.cpp
        src/MainDriver.cpp src/CommandProcessingDriver.cpp)

# Main executable
add_executable(COMP345_RISK             ${HEADERS} ${SOURCES} ${DRIVERS})

# Test executables
add_executable(CARDS_TEST               ${HEADERS} ${SOURCES} tests/CardsTest.cpp)
add_executable(COMMAND_PROCESSING_TEST  ${HEADERS} ${SOURCES} tests/CommandProcessingTest.cpp)
add_executable(GAME_ENGINE_TEST         ${HEADERS} ${SOURCES} tests/GameEngineTest.cpp)
add_executable(MAP_TEST                 ${HEADERS} ${SOURCES} tests/MapTest.cpp)
add_executable(ORDERS_TEST              ${HEADERS} ${SOURCES} tests/OrdersTest.cpp)
add_executable(PLAYER_TEST              ${HEADERS} ${SOURCES} tests/PlayerTest.cpp)

# Set Executable Properties
set_target_properties(CARDS_TEST               PROPERTIES CXX_STANDARD 20)
set_target_properties(COMMAND_PROCESSING_TEST  PROPERTIES CXX_STANDARD 20)
set_target_properties(GAME_ENGINE_TEST         PROPERTIES CXX_STANDARD 20)
set_target_properties(MAP_TEST                 PROPERTIES CXX_STANDARD 20)
set_target_properties(PLAYER_TEST              PROPERTIES CXX_STANDARD 20)
set_target_properties(ORDERS_TEST              PROPERTIES CXX_STANDARD 20)

# Linking test library to test executables
target_link_libraries(CARDS_TEST                gtest_main)
target_link_libraries(COMMAND_PROCESSING_TEST   gtest_main)
target_link_libraries(GAME_ENGINE_TEST          gtest_main)
target_link_libraries(MAP_TEST                  gtest_main)
target_link_libraries(ORDERS_TEST               gtest_main)
target_link_libraries(PLAYER_TEST               gtest_main)
