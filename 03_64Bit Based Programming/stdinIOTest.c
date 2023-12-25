#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define MAX_CMD_LENGTH 256

int CmdProcessing(void) {
    _fputts(_T("Input here...> "), stdout);

    // Use an array to store the input
    TCHAR cmdString[MAX_CMD_LENGTH];
    TCHAR* separators = _T(" \t\n"); 

    if (_fgetts(cmdString, MAX_CMD_LENGTH, stdin) != NULL) {
        // Process the input as needed
        _tprintf(_T("You entered: %s"), cmdString);

        // Tokenize the input if necessary
        
        TCHAR* token = _tcstok(cmdString, separators);
        UINT tokenNum = 0;

        while (token != NULL) {
            _tprintf(_T("Token[%u] => %s\n"), tokenNum, token);
            token = _tcstok(NULL, separators);
            ++tokenNum;
        }
    } else {
        _tprintf(_T("Error reading input.\n"));
    }

    return 0;
}

int _tmain(int argc, TCHAR* argv[]) {
    CmdProcessing();
    return 0;
}
