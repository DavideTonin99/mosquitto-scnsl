cmake_minimum_required(VERSION 3.10.2)
project(exam)

SET(CMAKE_CXX_STANDARD "17")
SET(CMAKE_CXX_EXTENSIONS "OFF")
set(CMAKE_CXX_FLAGS "-g ")

include_directories(include)
# file(GLOB SOURCES "src/*.cpp")
set(SOURCES src/main.cpp src/SCNSL_pub.cpp src/SCNSL_sub.cpp)
add_executable(exam ${SOURCES})
target_link_libraries(exam /home/davide/Scrivania/Network_simulation/mosquitto-scnsl-test/lib/lib-scnsl/libmosquitto.a scnsl systemc dl)
