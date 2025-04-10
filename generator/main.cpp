#include <iostream>
#include <source_location>
#include <filesystem>

#include <clang-c/Index.h>

using namespace std;

int main()
{
    auto location = source_location::current();
    cout << "Hello, " << location.file_name() << "\n";

    return 0;
}
