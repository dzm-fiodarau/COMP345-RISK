#_______________________________________________________________________________________________________________________
#   INCLUDES
include(FetchContent)



#_______________________________________________________________________________________________________________________
#   SETTING GLOBAL VARIABLES
cmake_minimum_required(VERSION 3.25)
project(COMP345_RISK VERSION 1.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enables testing through 'ctest'
enable_testing()



#_______________________________________________________________________________________________________________________
#   FETCHING DEPENDENCIES/LIBRARIES
# googletest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)



#_______________________________________________________________________________________________________________________
#   BUILD OPTIONS
# Toggle debug output macros.
option(DEBUG_MODE
       "Enables debugging code & macros."
       OFF)

# Isolates code to run game engine, DEBUG_MODE must be on.
option(EXECUTE_CARDS
       "Toggles execution of Cards code in debug mode."
       OFF)

option(EXECUTE_GAME_ENGINE
       "Toggles execution of GameEngine code in debug mode."
       OFF)

option(EXECUTE_MAP
       "Toggles execution of Map code in debug mode."
       OFF)

option(EXECUTE_ORDERS
       "Toggles execution of Orders code in debug mode."
       OFF)

option(EXECUTE_PLAYER
       "Toggles execution of Player code in debug mode."
       OFF)

option(EXECUTE_COMMAND_PROCESSING
       "Toggles execution of Command Processing code in debug mode."
       OFF)

# Determines whether the test executables should be (re-)compiled.
option(COMPILE_TESTS
       "Enables compilation of tests. Greatly increases compilation time of main application."
       ON)

#_______________________________________________________________________________________________________________________
#   PROCESSING OPTIONS
if(DEBUG_MODE)
    add_definitions(-DDEBUG -DRED_DEBUG_OUTPUT)
endif()

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



#_______________________________________________________________________________________________________________________
#   BUILDING FILES
# Defining the variables for the source files
set(HEADERS
        headers/Cards.h headers/gameengine/GameEngine.h headers/Map.h headers/Orders.h
        headers/commandprocessing/CommandProcessing.h headers/gameengine/State.h headers/gameengine/TransitionFunctions.h
        headers/macros/DebugMacros.h headers/commandprocessing/ConsoleCommandProcessorAdapter.h
        headers/commandprocessing/FileCommandProcessorAdapter.h
        headers/player/Player.h headers/player/AggressivePlayerStrategy.h headers/player/BenevolentPlayerStrategy.h
        headers/player/CheaterPlayerStrategy.h headers/player/HumanPlayerStrategy.h headers/player/NeutralPlayerStrategy.h
        headers/player/PlayerStrategies.h)

set(SOURCES
        src/Cards.cpp src/gameengine/GameEngine.cpp src/Map.cpp src/Orders.cpp src/commandprocessing/CommandProcessing.cpp
        src/gameengine/State.cpp src/gameengine/TransitionFunctions.cpp src/commandprocessing/ConsoleCommandProcessorAdapter.cpp
        src/commandprocessing/FileCommandProcessorAdapter.cpp
        src/player/Player.cpp src/player/AggressivePlayerStrategy.cpp src/player/BenevolentPlayerStrategy.cpp
        src/player/CheaterPlayerStrategy.cpp src/player/HumanPlayerStrategy.cpp src/player/NeutralPlayerStrategy.cpp
        src/player/PlayerStrategies.cpp)

set(DRIVERS
        src/drivers/CardsDriver.cpp src/drivers/GameEngineDriver.cpp src/drivers/MapDriver.cpp src/drivers/OrdersDriver.cpp src/drivers/PlayerDriver.cpp
        src/drivers/MainDriver.cpp src/drivers/CommandProcessingDriver.cpp)

# Main executable
add_executable(COMP345_RISK             ${HEADERS} ${SOURCES} ${DRIVERS})


# Test executables
# Generates build files if option is enabled
if(COMPILE_TESTS)
    # Test executables
    add_executable(CARDS_TEST_EXEC               ${HEADERS} ${SOURCES} tests/CardsTest.cpp)
    add_executable(COMMAND_PROCESSING_TEST_EXEC  ${HEADERS} ${SOURCES} tests/CommandProcessingTest.cpp)
    add_executable(GAME_ENGINE_TEST_EXEC         ${HEADERS} ${SOURCES} tests/GameEngineTest.cpp)
    add_executable(MAP_TEST_EXEC                 ${HEADERS} ${SOURCES} tests/MapTest.cpp)
    add_executable(ORDERS_TEST_EXEC              ${HEADERS} ${SOURCES} tests/OrdersTest.cpp)
    add_executable(PLAYER_TEST_EXEC              ${HEADERS} ${SOURCES} tests/PlayerTest.cpp)

    # Set Executable Properties
    set_target_properties(CARDS_TEST_EXEC               PROPERTIES CXX_STANDARD 20)
    set_target_properties(COMMAND_PROCESSING_TEST_EXEC  PROPERTIES CXX_STANDARD 20)
    set_target_properties(GAME_ENGINE_TEST_EXEC         PROPERTIES CXX_STANDARD 20)
    set_target_properties(MAP_TEST_EXEC                 PROPERTIES CXX_STANDARD 20)
    set_target_properties(ORDERS_TEST_EXEC              PROPERTIES CXX_STANDARD 20)
    set_target_properties(PLAYER_TEST_EXEC              PROPERTIES CXX_STANDARD 20)

    # Linking test library to test executables
    target_link_libraries(CARDS_TEST_EXEC                gtest_main)
    target_link_libraries(COMMAND_PROCESSING_TEST_EXEC   gtest_main)
    target_link_libraries(GAME_ENGINE_TEST_EXEC          gtest_main)
    target_link_libraries(MAP_TEST_EXEC                  gtest_main)
    target_link_libraries(ORDERS_TEST_EXEC               gtest_main)
    target_link_libraries(PLAYER_TEST_EXEC               gtest_main)

    # Update/set the test executables as a test in CMake
    add_test(NAME cards-test COMMAND CARDS_TEST_EXEC)
    add_test(NAME command-processing-test COMMAND COMMAND_PROCESSING_TEST_EXEC)
    add_test(NAME game-engine-test COMMAND GAME_ENGINE_TEST_EXEC)
    add_test(NAME map-test COMMAND MAP_TEST_EXEC)
    add_test(NAME orders-test COMMAND ORDERS_TEST_EXEC)
    add_test(NAME player-test COMMAND PLAYER_TEST_EXEC)
endif()
