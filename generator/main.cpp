#include <filesystem>
#include <iostream>
#include <source_location>

#include <clang-c/Index.h>

#include "Argument.h"
#include "Function.h"

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

static std::vector<Function> parseHeader(const fs::path &path)
{
    cout << "*** Parsing: " << path << "\n";

    CXIndex index = clang_createIndex(0, 0);

    CXTranslationUnit unit = clang_parseTranslationUnit(index, path.string().c_str(), nullptr, 0,
                                                        nullptr, 0, CXTranslationUnit_None);

    if (unit == nullptr)
    {
        cerr << "*** Unable to parse " << path << "\n";
        return {};
    }

    std::vector<Function> functions;

    CXCursor cursor = clang_getTranslationUnitCursor(unit);

    clang_visitChildren(
        cursor, // Root cursor
        [](CXCursor currentCursor, CXCursor parentCursor, CXClientData clientData) {
            std::vector<Function> *pFunctions = static_cast<std::vector<Function> *>(clientData);

            CXCursorKind cursorKind = clang_getCursorKind(currentCursor);

            switch (cursorKind)
            {
            case CXCursor_StructDecl: {
                CXString currentDisplayName = clang_getCursorDisplayName(currentCursor);
                string structName{clang_getCString(currentDisplayName)};
                clang_disposeString(currentDisplayName);

                if (structName.starts_with("SDL_"))
                {
                    structNames.push_back(structName);
                }

                break;
            }

            case CXCursor_FunctionDecl: {
                Function fn{currentCursor};

                if (fn.Name().starts_with("SDL_"))
                {
                    int numArgs = clang_Cursor_getNumArguments(currentCursor);
                    if (numArgs != -1)
                    {
                        for (int i = 0; i < numArgs; ++i)
                        {
                            CXCursor argCursor = clang_Cursor_getArgument(currentCursor, i);
                            fn.AddArgument(argCursor);
                        }
                    }

                    pFunctions->push_back(fn);
                }
                break;
            }
            }

            return CXChildVisit_Continue;
        },
        &functions // clientData
    );

    return functions;
}

static void output(const std::vector<Function> functions)
{
    for (const Function &fn : functions)
    {
        cout << fn.ReturnTypeString() << " " << fn.NamespacedName() << "(";
        cout << ")\n";
        cout << "{\n";
        cout << "}\n";
        cout << "\n";
    }
}

} // namespace zlang

int main()
{
    auto location = source_location::current();

    auto outputDirectory = fs::path{location.file_name()}.parent_path().parent_path();
    auto sdlHeaderDirectory =
        fs::path{location.file_name()}.parent_path().parent_path() / "SDL" / "include" / "SDL3";

    auto sdlInitPath = sdlHeaderDirectory / "SDL_init.h";
    std::vector<zlang::Function> functions = zlang::parseHeader(sdlInitPath);
    zlang::output(functions);

    /*
    for (const auto &entry : fs::directory_iterator(sdlHeaderDirectory))
    {
        std::vector<zlang::Function> functions = zlang::parseHeader(entry.path());
        zlang::output(functions);
    }
    */

    return 0;
}
