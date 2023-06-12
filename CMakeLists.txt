
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

add_executable(Peggle ./source/juego.cpp ./source/lista.cpp ./source/peggle.cpp ./source/lectura.cpp ./source/poligono.cpp ./source/obstaculo.cpp ./source/vec2.cpp)
target_include_directories(Peggle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(SDL2 REQUIRED)
target_link_libraries(Peggle ${SDL2_LIBRARIES})
include_directories(Peggle ${SDL2_INCLUDE_DIRS})



# add_executable(testSFML ./.main_renderer.cpp src/ball.cpp src/vec2.cpp)
# target_include_directories(testSFML PRIVATE "${PROJECT_BINARY_DIR}" ${CMAKE_CURRENT_SOURCE_DIR}/include)
# target_link_libraries(testSFML sfml-graphics)

# Including SFML

# # Including CxxTest
#
# file(GLOB_RECURSE TEST_FOO_HEADERS test/poligonotest.h)
#
# find_package(CxxTest)
# if(CXXTEST_FOUND)
#     include_directories(${CXXTEST_INCLUDE_DIR})
#     enable_testing()
#     CXXTEST_ADD_TEST(unittest-poligon runner-poligon.cpp src/poligono.cpp ${TEST_FOO_HEADERS})
#     target_include_directories(unittest-poligon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# endif()
#

