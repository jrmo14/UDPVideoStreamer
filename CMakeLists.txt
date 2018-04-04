cmake_minimum_required(VERSION 3.9)
project(Streamer)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)

set(src PracticalSocket.cpp PracticalSocket.h Streamer.cpp Streamer.h)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)


add_executable(Streamer main.cpp ${src})
target_link_libraries(Streamer ${OpenCV_LIBS})