cmake_minimum_required(VERSION 3.10)
project(SDL-Hpp
    VERSION 3.2
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(SDL_STATIC "Build SDL as a static library" ON)

add_subdirectory(SDL EXCLUDE_FROM_ALL)

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} PUBLIC
  FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    FILES SDL.hpp
 )

 if(SDL_STATIC)
    target_link_libraries(${PROJECT_NAME} INTERFACE SDL3::SDL3-static)
else()
    target_link_libraries(${PROJECT_NAME} INTERFACE SDL3::SDL3)
endif()
