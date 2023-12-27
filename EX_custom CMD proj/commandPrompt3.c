// commandprompt_3.c
// the basic struct of custom command prompt

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <Windows.h>
#include <stdbool.h>
// #define _CRT_SECURE_NO_WARNINGS

// Overall variables
#define STR_LEN 256
#define CMD_TOKEN_NUM 10
TCHAR ERROR_CMD[] = _T("The term '%s' is not recognized as the name of a cmdlet, function, script file, or operable program.\n");

// Functions that the current project uses
int CmdProcessing(UINT tokenNum);
TCHAR* StrLower(TCHAR*);

UINT CmdTokenizeAfterRead();

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
// String tokenization by
// - " ": whitespace
// - ",": comma
// - "\t": tab
// - "\n": newline
TCHAR seperators[] = _T(" ,\t\n");

/// <summary>
///		The entry point of the function
/// </summary>
/// <param name="argc">The number of commandline parameters</param>
/// <param name="argv">An array of commandline string</param>
/// <returns>0 when it finishes running</returns>
int _tmain(int argc, TCHAR* argv[]) {

	// enable Korean localization
	_tsetlocale(LC_ALL, _T("Korean"));

	_tprintf(_T("hello!\n"));

	// If the argument is more than two, the first argument must be
	// caller signal "start". So, exclude it and use only preceding thing.
	// ex) "start echo hello world" => CmdProcessing("echo hello world");
	if (argc > 1) {
		for (int index = 1; index < argc; index++)
			_tcscpy(cmdTokenList[index - 1], argv[index]);
		CmdProcessing(argc - 1);
	}

	while (true) {
		UINT tokenNum = CmdTokenizeAfterRead();
		DWORD isExit = CmdProcessing(tokenNum);
		if (isExit == true) {
			_fputts(_T("Terminate command processing"), stdout);
			break;
		}
	}

	return 0;

}

/// <summary>
///  Receive the (command) input from the user, fill the data to cmdString
///  and tokenize it(to store cmdTokenList), and count the number of tokenized commands.
/// </summary>
/// <returns>tokenNum(the number of return values)</returns>
UINT CmdTokenizeAfterRead() {

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

	return tokenNum;
}

/// <summary>
///		Received a command and execute the prescribed features.
///		If "exit" is received as a command, it will return "true" to the _tmain()) and terminate the program
///		according to the code within _tmain
/// </summary>
/// <param name=""></param>
/// <returns>true only if the terminal program is about to exit</returns>
int CmdProcessing(UINT tokenNum) {

	// In case of doing createProcess()
	STARTUPINFO startupInformation = { 0, };
	PROCESS_INFORMATION processInformation;
	startupInformation.cb = sizeof(startupInformation);

	// Because given instruction is entirely uncapitalized,
	// use the command with only using uncapitalized characters (e.g. File(X) => file(O))
	//  [command]		[arg1]			[arg2]			[arg3] ...
	//  cmdTokenList[0] cmdTokenList[1] cmdTokenList[2] cmdTokenList[3] ...
	if (!(_tcscmp(cmdTokenList[0], _T("exit"))))
		return true;

	// Just echoing message
	else if (!(_tcscmp(cmdTokenList[0], _T("echo")))) {
		for (size_t index = 1; index < CMD_TOKEN_NUM; index++)
			_tprintf(_T("%s "), cmdTokenList[index]);
		_fputts(_T("\n"), stdout);
	}

	// create a new command line console
	else if (!(_tcscmp(cmdTokenList[0], _T("start")))) {
		TCHAR currentProcessPath[MAX_PATH] = { 0, };
		GetModuleFileName(NULL, currentProcessPath, MAX_PATH);

		// suppose that the command "start echo hello world" is received.
		// then, run a new console process and execute "echo hello world" instruction
		TCHAR createProcessStringCommand[STR_LEN] = { 0, };
		_stprintf(createProcessStringCommand, _T("%s"), currentProcessPath);

		for (UINT index = 1; index < tokenNum; index++) {
			_tcscat(createProcessStringCommand, _T(" "));
			_tcscat(createProcessStringCommand, cmdTokenList[index]);
		}

		BOOL state = CreateProcess(
			NULL,
			createProcessStringCommand,
			NULL,
			NULL,
			TRUE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&startupInformation,
			&processInformation
		);

		if (state != false)
			_tprintf(_T("Successfully executed the command \"%s\" remotely.\n"), cmdString);

		CloseHandle(processInformation.hThread);
		CloseHandle(processInformation.hProcess);

	}

	else if (!(_tcscmp(cmdTokenList[0], _T("owo")))) {
		_fputts(_T("UwU\n"), stdout);
	}
	// if there is nothing received, just immediately finish
	else if (!(_tcscmp(cmdTokenList[0], _T("")))) {
		return 0;
	}
	else {
		// Seems that the custom console need to search and execute basic WINDOWS32 application.
		TCHAR* command = cmdString;
		SetCurrentDirectory(_T("C:\\WINDOWS\\system32"));
		BOOL state = CreateProcess(
			NULL,						// The name of process
			command,					// The name of executable
			NULL,
			NULL,
			TRUE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&startupInformation,
			&processInformation
		);

		CloseHandle(processInformation.hThread);
		CloseHandle(processInformation.hProcess);

		if (state != false) {
			// Let the user know that the instruction successfully accomplished.
			// To regularly format the path, just delete the extension part.
			TCHAR* executionTarget = command;
			TCHAR* lastDot = _tcsrchr(executionTarget, '.');
			if (lastDot != NULL)
				*lastDot = _T('\0');

			_tprintf(_T("C:\\windows\\system32\\%s.exe executed.\n"), executionTarget);
		}
		else
			// The request program actually doesn't exist, thus the process creation failed.
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