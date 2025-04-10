# SDL-Hpp (SDL.hpp)

[Single C++ header file for SDL3](SDL.hpp), offering built-in error handling via exceptions
and a [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr)-like
`UniquePointer` for SDL types.

Has SDL "vendored-in" via git submodules if that is wanted.

## Status

The current [SDL.hpp](SDL.hpp) is a short proof-of-concept implementation of a tiny
subset of the API. Not usable yet unless you want to add all the functions
you need manually. Which is not the plan.

The [generator](generator) is under development, using [libclang](https://clang.llvm.org/)
to parse [SDL3/SDL.h](https://github.com/libsdl-org/SDL/blob/main/include/SDL3/SDL.h)
and emit a complete c++ SDL3 API.

## Usage with CMake

For example:

* Add the project as a git submodule.
* In your `CMakeLists.txt`, set `SDL_STATIC` or offer it as option.
* Add the project: `add_subdirectory(SDL-Hpp)`.
* Link to it: `target_link_libraries(${PROJECT_NAME} PRIVATE SDL-Hpp)`.
* Include it like this: `#include <SDL.hpp>`.