#include <filesystem>
#include <iostream>
#include <source_location>

#include <clang-c/Index.h>

using namespace std;
namespace fs = std::filesystem;

void parseHeader(const fs::path &path)
{
    CXIndex index = clang_createIndex(0, 0);

    CXTranslationUnit unit = clang_parseTranslationUnit(index, path.string().c_str(), nullptr, 0,
                                                        nullptr, 0, CXTranslationUnit_None);

    if (unit == nullptr)
    {
        std::cerr << "Unable to parse translation unit. Quitting.\n";
        return;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);

    clang_visitChildren(
        cursor, // Root cursor
        [](CXCursor current_cursor, CXCursor parent, CXClientData client_data) {
            CXString current_display_name = clang_getCursorDisplayName(current_cursor);
            // Allocate a CXString representing the name of the current cursor

            std::cout << "Visiting element " << clang_getCString(current_display_name) << "\n";
            // Print the char* value of current_display_name

            clang_disposeString(current_display_name);
            // Since clang_getCursorDisplayName allocates a new CXString, it must be freed. This
            // applies to all functions returning a CXString

            return CXChildVisit_Recurse;
        },      // CXCursorVisitor: a function pointer
        nullptr // client_data
    );
}

int main()
{
    auto location = source_location::current();

    auto outputDirectory = fs::path{location.file_name()}.parent_path().parent_path();
    auto sdlHeaderDirectory =
        fs::path{location.file_name()}.parent_path().parent_path() / "SDL/include/SDL3";

    for (const auto &entry : fs::directory_iterator(sdlHeaderDirectory))
    {
        cout << "*** Parsing: " << entry.path() << "\n";
        parseHeader(entry.path());
    }

    return 0;
}
