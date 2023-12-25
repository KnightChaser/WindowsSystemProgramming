// commandprompt_1.c
// the basic struct of custom command prompt

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <Windows.h>
#include <stdbool.h>
#define _CRT_SECURE_NO_WARNINGS

// Overall variables
#define STR_LEN 256
#define CMD_TOKEN_NUM 10
TCHAR ERROR_CMD[] = _T("The term '%s' is not recognized as the name of a cmdlet, function, script file, or operable program.\n");

// Functions that the current project uses
int CmdProcessing(void);
TCHAR* StrLower(TCHAR*);

/// <summary>
///		The entry point of the function
/// </summary>
/// <param name="argc">The number of commandline parameters</param>
/// <param name="argv">An array of commandline string</param>
/// <returns>0 when it finishes running</returns>
int _tmain(int argc, TCHAR* argv[]) {

	// enable Korean localization
	_tsetlocale(LC_ALL, _T("Korean"));

	DWORD isExit;
	while (true) {
		isExit = CmdProcessing();
		if (isExit == true) {
			_fputts(_T("Terminate command processing"), stdout);
			break;
		}
	}

	return 0;

}

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seperators[] = _T(",\t\n");

/// <summary>
///		Received a command and execute the prescribed features.
///		If "exit" is received as a command, it will return "true" to the _tmain()) and terminate the program
///		according to the code within _tmain
/// </summary>
/// <param name=""></param>
/// <returns>true only if the terminal program is about to exit</returns>
int CmdProcessing(void) {
	_fputts(_T("commandHere@UwU:~$ "), stdout);

	// refresh the buffer and restart
	memset(cmdString, 0, sizeof(cmdString));
	memset(cmdTokenList, 0, sizeof(cmdTokenList));
	_fgetts(cmdString, STR_LEN, stdin);
	TCHAR* token = _tcstok(cmdString, seperators);
	UINT tokenNum = 0;

	// uncapitalize every tokenized command string to unify the command selection process
	// ex) openFile, OpenFILE, OpenFile, OPENFILE => openfile (uncapialized)
	while (token != NULL) {
		_tcscpy(cmdTokenList[tokenNum], StrLower(token));
		tokenNum++;
		token = _tcstok(NULL, seperators);
	}

	// Because given instruction is entirely uncapitalized,
	// use the command with only using uncapitalized characters (e.g. File(X) => file(O))
	//  [command]		[arg1]			[arg2]			[arg3] ...
	//  cmdTokenList[0] cmdTokenList[1] cmdTokenList[2] cmdTokenList[3] ...
	if (!(_tcscmp(cmdTokenList[0], _T("exit"))))
		return true;
	else if (!(_tcscmp(cmdTokenList[0], _T("future command 1")))) {
		// ...
	}
	else if (!(_tcscmp(cmdTokenList[0], _T("future command 2")))) {
		// ...
	}
	else if (!(_tcscmp(cmdTokenList[0], _T("owo")))) {
		_fputts(_T("UwU\n"), stdout);
	}
	// if there is nothing received, just immediately finish
	else if (!(_tcscmp(cmdTokenList[0], _T("")))) {
		return 0;
	}
	else {
		_tprintf(ERROR_CMD, cmdTokenList[0]);
	}

	return 0;
}


/// <summary>
///		Change every capitalized characters in the given string to the uncapitalized (e.g. ABxyKK9a => abxykk9a)
///		and returns the string pointer that is processed. It will handle for unexpectedly capitalized commands like
///		"OpenFile" instead of "openfile". (Suppose that there is an instruction feature "openfile".
/// </summary>
/// <param name="ptrString">The string(TCHAR*) pointer that needs to be uncapitalized</param>
/// <returns>The string(TCHAR*) pointer that has been uncapitalized (the given string pointer is now modified)</returns>
TCHAR* StrLower(TCHAR* ptrString) {

	// Copy original starting pointer of the given string to return
	TCHAR* targetString = ptrString;

	while (*ptrString) {
		if (_istupper(*ptrString))
			*ptrString = _totlower(*ptrString);
		ptrString++;
	}

	// the string that TCHAR* targetString directs processed.
	return targetString;
}