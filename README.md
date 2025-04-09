# SDL-Hpp (SDL.hpp)

C++ header file for SDL3, offering built-in error handling via exceptions
and a `std::unique_ptr`-like `UniquePointer` for SDL types.

Currently implemented manually step-by-step to test out the waters,
but should be generated
later using [libclang](https://clang.llvm.org/), in order to give
complete API coverage.

As such, it is currently INCOMPLETE but in use (for driving the implementation).

Has SDL "vendored-in" via git submodules if that is wanted.

## Usage with CMake

For example:

* Add the project as a git submodule.
* In your `CMakeLists.txt`, set `SDL_STATIC` or offer it as option.
* Add the project: `add_subdirectory(SDL-Hpp)`.
* Link to it: `target_link_libraries(${PROJECT_NAME} PRIVATE SDL-Hpp)`.
* Include it like this: `#include <SDL.hpp>`.