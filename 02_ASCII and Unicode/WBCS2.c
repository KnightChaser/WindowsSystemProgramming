// WBCS fputs
#include <stdio.h>

// main() function for wchar_t(unicode) set
int wmain(int argc, wchar_t* argv[]) {

    for (unsigned int index = 1; index < argc; index++) {
        wprintf(L"argv[%d] => %s", index, argv[index]);
        fputws(L"\n", stdout);
    }

    return 0;
}
