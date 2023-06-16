
cmake_minimum_required(VERSION 3.12)
project(Peggle VERSION 2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug
    CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Export compile commands for completion engines (optional)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Peggle source/juego.cpp ./source/level.cpp source/renderer.cpp source/peggle.cpp source/cannon.cpp source/poligono.cpp source/obstaculo.cpp source/vec2.cpp source/bola.cpp source/vidas.cpp source/recuperador.cpp source/trayectoria.cpp source/loader.cpp)
target_include_directories(Peggle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(SDL2 REQUIRED)
target_link_libraries(Peggle SDL2_ttf) # hacer opcional
target_link_libraries(Peggle ${SDL2_LIBRARIES})
include_directories(Peggle ${SDL2_INCLUDE_DIRS})
