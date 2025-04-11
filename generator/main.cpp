#include <filesystem>
#include <iostream>
#include <source_location>

#include <clang-c/Index.h>

#include "Parameter.h"

using namespace std;
namespace fs = std::filesystem;

namespace zlang
{

vector<string> structNames{};

static string ctypeString(CXType cursorType)
{
    if (cursorType.kind == CXType_Pointer)
    {
        CXType pointeeType = clang_getPointeeType(cursorType);
        return ctypeString(pointeeType) + " *";
    }
    else
    {
        CXString typeStringCX = clang_getTypeKindSpelling(cursorType.kind);
        string s{clang_getCString(typeStringCX)};
        clang_disposeString(typeStringCX);

        return s;
    }
}

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
        [](CXCursor currentCursor, CXCursor parentCursor, CXClientData clientData) {
            CXChildVisitResult result = CXChildVisit_Continue;

            CXString currentDisplayName = clang_getCursorDisplayName(currentCursor);

            CXCursorKind cursorKind = clang_getCursorKind(currentCursor);
            switch (cursorKind)
            {
            case CXCursor_StructDecl: {
                string structName{clang_getCString(currentDisplayName)};
                if (structName.starts_with("SDL_"))
                {
                    cout << "struct " << structName << "\n";
                    structNames.push_back(structName);
                }
                break;
            }

            case CXCursor_FunctionDecl: {
                string functionDecl{clang_getCString(currentDisplayName)};

                CXString functionName = clang_getCursorSpelling(currentCursor);
                string functionNameString{clang_getCString(functionName)};
                clang_disposeString(functionName);

                if (functionNameString.starts_with("SDL_"))
                {
                    cout << "function: " << functionDecl << "\n";
                    int numArgs = clang_Cursor_getNumArguments(currentCursor);
                    if (numArgs != -1)
                    {
                        for (int i = 0; i < numArgs; ++i)
                        {
                            CXCursor argCursor = clang_Cursor_getArgument(currentCursor, i);

                            CXString argName = clang_getCursorDisplayName(argCursor);
                            string argNameString{clang_getCString(argName)};
                            clang_disposeString(argName);

                            CXType argCursorType = clang_getCursorType(argCursor);
                            string argTypeString = ctypeString(argCursorType);

                            CXString pretty = clang_getCursorPrettyPrinted(argCursor, nullptr);
                            string prettyString{clang_getCString(pretty)};
                            clang_disposeString(pretty);

                            cout << "  arg: " << prettyString << "\n";
                        }
                    }

                    result = CXChildVisit_Recurse;
                }
                break;
            }

            case CXCursor_ParmDecl: {
                string paramDecl{clang_getCString(currentDisplayName)};
                cout << "  param: " << paramDecl << "\n";
                break;
            }
            }

            clang_disposeString(currentDisplayName);

            return result;
        },
        nullptr // clientData
    );

    return true;
}

} // namespace zlang

int main()
{
    auto location = source_location::current();

    auto outputDirectory = fs::path{location.file_name()}.parent_path().parent_path();
    auto sdlHeaderDirectory =
        fs::path{location.file_name()}.parent_path().parent_path() / "SDL" / "include" / "SDL3";

    auto sdlInitPath = sdlHeaderDirectory / "SDL_init.h";
    zlang::parseHeader(sdlInitPath);

    /*
    for (const auto &entry : fs::directory_iterator(sdlHeaderDirectory))
    {
        zlang::parseHeader(entry.path());
    }
    */

    return 0;
}
