#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <source_location>

#include <clang-c/Index.h>

#include "Argument.h"
#include "Function.h"

using namespace std;
namespace fs = std::filesystem;

namespace zlang
{

vector<string> structNames{};

static std::vector<Function> ParseHeader(const fs::path &path,
                                         const std::vector<std::string> &includePaths)
{
    CXIndex index = clang_createIndex(0, 1);

    std::vector<const char *> paths;
    for (const std::string &includePath : includePaths)
    {
        paths.push_back(includePath.c_str());
    }

    CXTranslationUnit unit = clang_parseTranslationUnit(index, path.string().c_str(), paths.data(),
                                                        static_cast<int>(paths.size()), nullptr, 0,
                                                        CXTranslationUnit_None);

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

static void OutputFunctionArguments(std::ostream &out, const Function &fn, bool namesOnly)
{
    bool haveOutputAtLeastOneFunctionArgument = false;
    for (const Argument &arg : fn.Arguments())
    {
        if (haveOutputAtLeastOneFunctionArgument)
        {
            out << ", ";
        }
        if (namesOnly)
        {
            out << arg.Name();
        }
        else
        {
            out << arg.Declaration();
        }
        haveOutputAtLeastOneFunctionArgument = true;
    }
}

static void OutputDestructors(std::ostream &out, const std::vector<Function> functions)
{
    std::set<std::string> alreadyGenerated{};
    std::set<std::string> functionsToIgnore{
        "SDL_MemoryBarrierReleaseFunction", "SDL_DestroyHapticEffect", "SDL_DestroyProperties",
        "SDL_ReleaseCameraFrame",           "SDL_GL_DestroyContext",   "SDL_Metal_DestroyView"};

    for (const Function &fn : functions)
    {
        if (fn.Name().find("Destroy") != std::string::npos ||
            fn.Name().find("Release") != std::string::npos)
        {
            bool handled = false;

            if (fn.NumberOfArguments() == 1)
            {
                const Argument &arg = fn.Arguments()[0];
                std::string pointedType = arg.PointeeTypeString();

                if (arg.IsPointer() && !alreadyGenerated.contains(pointedType))
                {
                    alreadyGenerated.insert(pointedType);

                    out << "template<>\n";
                    out << "void Destroy<" << pointedType << ">(" << arg.Declaration() << ")\n";
                    out << "{\n";
                    out << "    " << fn.Name() << "(" << arg.Name() << ");\n";
                    out << "}\n\n";

                    std::string typeString =
                        pointedType.starts_with("SDL_") ? pointedType.substr(4) : pointedType;
                    out << "using " << typeString << " = UniquePointer<" << pointedType << ">;\n\n";

                    handled = true;
                }
                else if (alreadyGenerated.contains(pointedType))
                {
                    handled = true;
                }
            }
            else if (fn.NumberOfArguments() == 2)
            {
                const Argument &arg1 = fn.Arguments()[0];
                const Argument &arg2 = fn.Arguments()[1];
                std::string pointedType1 = arg1.PointeeTypeString();
                std::string pointedType2 = arg2.PointeeTypeString();
                if (pointedType1 == "SDL_GPUDevice")
                {
                    out << "template<>\n";
                    out << "void ReleaseFromDevice<" << pointedType2 << ">(" << arg1.Declaration()
                        << "," << arg2.Declaration() << ")\n";
                    out << "{\n";
                    out << "    " << fn.Name() << "(" << arg1.Name() << ", " << arg2.Name()
                        << ");\n";
                    out << "}\n\n";

                    handled = true;
                }
            }

            if (!handled && !functionsToIgnore.contains(fn.Name()))
            {
                cout << "Unhandled release function: " << fn.Name() << "\n";
            }
        }
    }
}

static void OutputFunctions(std::ostream &out, const std::vector<Function> functions)
{
    std::set<std::string> functionsToSkip{"SDL_size_mul_check_overflow_builtin",
                                          "SDL_size_add_check_overflow_builtin"};

    for (const Function &fn : functions)
    {
        if (fn.HasSDLPrefix() && !functionsToSkip.contains(fn.Name()))
        {
            std::string returnType = fn.ReturnsBool() ? "void" : fn.ReturnTypeString();
            std::string space = fn.ReturnsPointer() ? "" : " ";

            out << "inline " << returnType << space << fn.NamespacedName() << "(";
            OutputFunctionArguments(out, fn, false);

            if (!fn.IsUnchecked())
            {
                if (fn.NumberOfArguments() > 0)
                {
                    out << ", ";
                }

                out << "std::source_location location = std::source_location::current()";
            }

            out << ")\n";
            out << "{\n";

            if (fn.IsUnchecked())
            {
                if (!fn.IsVoid())
                {
                    out << "    return ";
                }
                else
                {
                    out << "    ";
                }
                out << fn.Name() << "(";
                OutputFunctionArguments(out, fn, true);
                out << ");\n";
            }
            else if (fn.ReturnsBool())
            {
                out << "    if (!" << fn.Name() << "(";
                OutputFunctionArguments(out, fn, true);
                out << "))\n" << "    {\n";
                out << "        SDLThrow(location);\n";
                out << "    }\n";
            }
            else if (fn.ReturnsPointer())
            {
                out << "    " << fn.ReturnTypeString() << "result = " << fn.Name() << "(";
                OutputFunctionArguments(out, fn, true);
                out << ");\n";
                out << "    if (!result)\n    {\n        SDLThrow(location);\n    }\n";
                out << "    return result;\n";
            }

            out << "}\n";
            out << "\n";
        }
    }
}

} // namespace zlang

using namespace zlang;

int main()
{
    auto location = source_location::current();
    auto locationPath = fs::path{location.file_name()}.parent_path();
    auto outputDirectory = fs::path{location.file_name()}.parent_path().parent_path();

    auto outputFile = outputDirectory / "SDL.hpp";
    auto prologueFile = locationPath / "Prologue.h";
    auto epilogueFile = locationPath / "Epilogue.h";

    std::ofstream out{outputFile};

    std::ifstream ifsPrologue{prologueFile};
    std::string line;
    while (std::getline(ifsPrologue, line))
    {
        out << line << "\n";
    }
    out << "\n";
    ifsPrologue.close();

    auto sdlIncludePath =
        fs::path{location.file_name()}.parent_path().parent_path() / "SDL" / "include";
    auto sdlHeaderDirectory = sdlIncludePath / "SDL3";

    std::string includePath1 = "-I" + sdlIncludePath.string();
    auto sdlIncludeFile = sdlHeaderDirectory / "SDL.h";

    std::vector<Function> functions = ParseHeader(sdlIncludeFile, {includePath1});
    OutputDestructors(out, functions);
    OutputFunctions(out, functions);

    std::ifstream ifsEpilogue{epilogueFile};
    while (std::getline(ifsEpilogue, line))
    {
        out << line << "\n";
    }
    ifsEpilogue.close();

    out.close();

    return 0;
}
