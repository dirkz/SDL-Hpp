#include <filesystem>
#include <iostream>
#include <source_location>

#include <clang-c/Index.h>

using namespace std;

int main()
{
    auto location = source_location::current();
    auto path = filesystem::path{location.file_name()}.parent_path().parent_path() /
                "SDL/include/SDL3/SDL.h";

    CXIndex index = clang_createIndex(0, 0);

    CXTranslationUnit unit = clang_parseTranslationUnit(index, path.string().c_str(), nullptr, 0,
                                                        nullptr, 0, CXTranslationUnit_None);

    if (unit == nullptr)
    {
        std::cerr << "Unable to parse translation unit. Quitting.\n";
        return 0;
    }
    CXCursor cursor = clang_getTranslationUnitCursor(unit);

    return 0;
}
