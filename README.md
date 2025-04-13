# SDL-Hpp (SDL.hpp)

[Single C++ header file for SDL3](SDL.hpp), offering built-in error handling via exceptions
and a [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr)-like
`UniquePointer` for SDL types.

Has SDL "vendored-in" via git submodules if that is wanted.

The [generator](generator) project uses [libclang](https://clang.llvm.org/)
to parse [SDL3/SDL.h](https://github.com/libsdl-org/SDL/blob/main/include/SDL3/SDL.h)
and emits a complete C++ SDL3 API.

## Status

Looks usable, but has to be put under test in a real project in order
to find out what is missing or not correctly generated.

## Naming

A type like [SDL_Window](https://wiki.libsdl.org/SDL3/SDL_Window) becomes a  `Window`,
which is a [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr)-like
type, that is, it destroys the `SDL_Window` when it goes out of scope.

## Usage with CMake

For example:

* Add the project as a git submodule.
* In your `CMakeLists.txt`, set `SDL_STATIC` (BOOL) or offer it as option.
* Add the project: `add_subdirectory(SDL-Hpp)`.
* Link to it: `target_link_libraries(${PROJECT_NAME} PRIVATE SDL-Hpp)`.
* Include it like this: `#include <SDL.hpp>`.