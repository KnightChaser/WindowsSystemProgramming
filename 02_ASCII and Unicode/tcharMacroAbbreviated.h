#ifdef _UNICODE
    #define _tmain      wmain
    #define _tcslen     wcslen
    #define _tcscat     wcscat
    #define _tcscpy     wcscpy
    #define _tcsncpy    wcsncpy
    #define _tcscmp     wcscmp
    #define _tcsncmp    wcsncmp
    #define _tprintf    wprintf
    #define _tscanf     wscanf
    #define _fgetts     fgetws
    #define _fputts     fputws
#else
    #define _tmain      main
    #define _tcslen     strlen
    #define _tcscat     strcat
    #define _tcscpy     strcpy
    #define _tcsncpy    strncpy
    #define _tcscmp     strcmp
    #define _tcsncmp    strncmp
    #define _tprintf    printf
    #define _tscanf     scanf
    #define _fgetts     fgets
    #define _fputts     fputs
#endif