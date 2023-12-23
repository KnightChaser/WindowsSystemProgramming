// WBCS fputs
#include <stdio.h>

// main() function for wchar_t(unicode) set
int wmain(int argc, wchar_t* argv[]) {

    for(int i = 1; i < argc; i++) {
        fputws(argv[i], stdout);
        fputws(L"\n", stdout);
    }

    return 0;
}
