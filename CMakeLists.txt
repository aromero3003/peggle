
cmake_minimum_required(VERSION 3.12)
project(Peggle VERSION 2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug
    CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Export compile commands for completion engines (optional)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SOURCES 
  source/vec2.cpp
  source/ball.cpp
  source/game.cpp
  source/main.cpp
  source/lifes.cpp
  source/level.cpp
  source/loader.cpp
  source/peggle.cpp
  source/cannon.cpp
  source/polygon.cpp
  source/renderer.cpp
  source/obstacle.cpp
  source/retriever.cpp
  source/trajectory.cpp
  source/hybrid_renderer.cpp
)
add_executable(Peggle ${SOURCES})
target_include_directories(Peggle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(SDL2 REQUIRED)
target_link_libraries(Peggle SDL2_ttf) # hacer opcional
target_link_libraries(Peggle ${SDL2_LIBRARIES})
include_directories(Peggle ${SDL2_INCLUDE_DIRS})
