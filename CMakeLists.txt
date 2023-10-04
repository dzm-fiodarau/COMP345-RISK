cmake_minimum_required(VERSION 3.25)
project(COMP345_RISK)

set(CMAKE_CXX_STANDARD 14)

add_executable(COMP345_RISK
        include/GameEngine.h
        src/GameEngine.cpp
        src/GameEngineDriver.cpp)
