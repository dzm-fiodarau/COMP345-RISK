cmake_minimum_required(VERSION 3.25)
project(COMP345_RISK)

set(CMAKE_CXX_STANDARD 14)

add_executable(COMP345_RISK
        headers/Cards.h
	headers/GameEngine.h
	headers/Map.h
	headers/Orders.h
	headers/Player.h
        src/Cards.cpp
        src/CardsDriver.cpp
	src/GameEngine.cpp
	src/GameEngineDriver.cpp
	src/MainDriver.cpp
	src/Map.cpp
	src/MapDriver.cpp
	src/Orders.cpp
	src/OrdersDriver.cpp
	src/Player.cpp
	src/PlayerDriver.cpp)
