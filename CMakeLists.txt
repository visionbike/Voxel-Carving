cmake_minimum_required(VERSION 3.8)
project(Voxel_Carving)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp camera.hpp camera.cpp voxel_grid.hpp voxel_grid.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})