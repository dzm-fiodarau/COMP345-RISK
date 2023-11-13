#_______________________________________________________________________________________________________________________
#   SETTING GLOBAL VARIABLES
cmake_minimum_required(VERSION 3.25)
project(COMP345_RISK VERSION 1.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)



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
file(GLOB HEADERS_BASE                "headers/*.h")
file(GLOB HEADERS_COMMAND_PROCESSING  "headers/commandprocessing/*.h")
file(GLOB HEADERS_DRIVERS             "headers/gameengine/*.h")
file(GLOB HEADERS_GAME_ENGINE         "headers/macros/*.h")
file(GLOB HEADERS_PLAYER              "headers/player/*.h")

file(GLOB SOURCES_BASE               "src/*.cpp")
file(GLOB SOURCES_COMMAND_PROCESSING "src/commandprocessing/*.cpp")
file(GLOB SOURCES_DRIVERS            "src/drivers/*.cpp")
file(GLOB SOURCES_GAME_ENGINE        "src/gameengine/*.cpp")
file(GLOB SOURCES_PLAYER             "src/player/*.cpp")

set(HEADERS ${HEADERS_BASE} ${HEADERS_COMMAND_PROCESSING} ${HEADERS_DRIVERS} ${HEADERS_GAME_ENGINE} ${HEADERS_PLAYER})
set(SOURCES ${SOURCES_BASE} ${SOURCES_COMMAND_PROCESSING} ${SOURCES_DRIVERS} ${SOURCES_GAME_ENGINE} ${SOURCES_PLAYER})

# Main executable
add_executable(COMP345_RISK ${HEADERS} ${SOURCES})
#   target_include_directories(COMP345_RISK PRIVATE ${tclap_SOURCE_DIR}/include)