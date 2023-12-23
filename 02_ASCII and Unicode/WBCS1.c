// Wide Byte Character Set(WBCS) to Unicode
#include <stdio.h>
#include <string.h>
#include <locale.h>

int main(void) {

    // Unicode string (wchar_t, L"...")
    wchar_t str[] = L"ABC한글"; // Because it's unicode, every character will be 2 bytes including NULL
    _wsetlocale(LC_ALL, L"korean"); // set locale for korean character printing in wprintf()
    int size = sizeof(str);
    int len = wcslen(str);      // WBCS version of strlen()

    wprintf(L"Size of an array: %d\n", size);   // WBCS version of printf()
    wprintf(L"Length of a string: %d\n", len);
    wprintf(L"예시 한글 출력\n");

    return 0;

}