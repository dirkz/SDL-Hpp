# CMakeList.txt : CMake project for SDL-Generator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (SDL-Generator)

# Please see https://llvm.org/docs/CMake.html#embedding-llvm-in-your-project
# for how to configure this, or use a CMakeUserPresets.json
# (as in CMakeUserPresets.json_sample).
find_package(Clang REQUIRED CONFIG)

include_directories(${CLANG_INCLUDE_DIRS})
add_definitions(${CLANG_DEFINITIONS})

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "main.cpp" "Parameter.h" "Parameter.cpp")

target_link_libraries(${PROJECT_NAME} libclang)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
