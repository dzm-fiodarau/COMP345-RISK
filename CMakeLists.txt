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

#   FetchContent_Declare(
#           tclap
#           GIT_REPOSITORY https://github.com/mirror/tclap
#           GIT_TAG        v1.2.5
#   )

FetchContent_MakeAvailable(googletest)
#   FetchContent_MakeAvailable(tclap)

#_______________________________________________________________________________________________________________________
#   BUILD OPTIONS
# Toggle debug output macros.
option(DEBUG_MODE
       "Enables debugging code & macros."
       OFF)



#_______________________________________________________________________________________________________________________
#   PROCESSING OPTIONS
if(DEBUG_MODE)
    add_definitions(-DDEBUG -DRED_DEBUG_OUTPUT)
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
#   target_include_directories(COMP345_RISK PRIVATE ${tclap_SOURCE_DIR}/include)