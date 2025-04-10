#include <filesystem>
#include <iostream>
#include <source_location>

#include <clang-c/Index.h>

using namespace std;
namespace fs = std::filesystem;

vector<string> structNames{};

static bool parseHeader(const fs::path &path)
{
    cout << "*** Parsing: " << path << "\n";

    CXIndex index = clang_createIndex(0, 0);

    CXTranslationUnit unit = clang_parseTranslationUnit(index, path.string().c_str(), nullptr, 0,
                                                        nullptr, 0, CXTranslationUnit_None);

    if (unit == nullptr)
    {
        cerr << "*** Unable to parse " << path << "\n";
        return false;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);

    clang_visitChildren(
        cursor, // Root cursor
        [](CXCursor currentCursor, CXCursor parent, CXClientData clientData) {
            CXString current_display_name = clang_getCursorDisplayName(currentCursor);

            bool shouldRecurse = false;

            CXCursorKind cursorKind = clang_getCursorKind(currentCursor);
            switch (cursorKind)
            {
            case CXCursor_StructDecl: {
                string structName{clang_getCString(current_display_name)};
                if (structName.starts_with("SDL_"))
                {
                    cout << "struct " << structName << "\n";
                    structNames.push_back(structName);
                }
                break;
            }

            case CXCursor_FunctionDecl: {
                string functionDecl{clang_getCString(current_display_name)};
                cout << "function " << functionDecl << "\n";
                shouldRecurse = true;
                break;
            }

            case CXCursor_ParmDecl: {
                CXType cursorType = clang_getCursorType(currentCursor);
                CXString paramTypeNameSpelling = clang_getTypeKindSpelling(cursorType.kind);
                string paramName{clang_getCString(current_display_name)};
                string paramTypeSpelling{clang_getCString(paramTypeNameSpelling)};
                cout << "  parameter " << paramName << ": " << paramTypeSpelling << "\n";
                clang_disposeString(paramTypeNameSpelling);
                shouldRecurse = true;
                break;
            }
            }

            clang_disposeString(current_display_name);

            if (shouldRecurse)
            {
                return CXChildVisit_Recurse;
            }
            else
            {
                return CXChildVisit_Continue;
            }
        },
        nullptr // clientData
    );

    return true;
}

int main()
{
    auto location = source_location::current();

    auto outputDirectory = fs::path{location.file_name()}.parent_path().parent_path();
    auto sdlHeaderDirectory =
        fs::path{location.file_name()}.parent_path().parent_path() / "SDL" / "include" / "SDL3";

    for (const auto &entry : fs::directory_iterator(sdlHeaderDirectory))
    {
        parseHeader(entry.path());
    }

    return 0;
}
