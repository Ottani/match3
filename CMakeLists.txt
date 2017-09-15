cmake_minimum_required (VERSION 3.6.2)
set(PROJECT_NAME "match3")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cpp gem_manager.cpp gem.cpp)

find_package(SFML REQUIRED graphics window system)

target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
