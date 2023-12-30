// commandprompt_4.c
// the basic struct of custom command prompt

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <Windows.h>
#include <stdbool.h>
#include <TlHelp32.h>
// #define _CRT_SECURE_NO_WARNINGS

// Overall variables
#define STR_LEN 256
#define CMD_TOKEN_NUM 10
TCHAR ERROR_CMD[] = _T("The term '%s' is not recognized as the name of a cmdlet, function, script file, or operable program.\n");

// Functions that the current project uses
int CmdProcessing(UINT tokenNum);
TCHAR* StrLower(TCHAR*);
UINT CmdTokenizeAfterRead();

BOOL listProcessInformation();
BOOL terminateProcess(TCHAR* targetProcessName);

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
	
	// list every process information
	else if (!(_tcscmp(cmdTokenList[0], _T("lp")))) {
		listProcessInformation();
	}

	// kill process like taskkill.exe
	else if (!(_tcscmp(cmdTokenList[0], _T("kp")))) {
		// first parameter(cmdTokenList[1]) should be entered as a target process name(e.g. CalculatorApp.exe) to kill
		// Capitalization will be ignored due to the program's tokenization. (Ref to terminateProcess())
		if (_countof(cmdTokenList[1]) > 0) {
			if (terminateProcess(cmdTokenList[1]))
				_fputts(_T("Operation successfully conducted.\n"), stdout);
			else
				_fputts(_T("Operation failed or abnormally exited.\n"), stdout);
		}
		else
			_fputts(_T("Wrong usage. Usage: kp [processNameToKill]\n"), stdout);
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

/// <summary>
///  Take a snapshot of the current process information in the device when this called by something else
///  and print every process information record in row
/// </summary>
/// <returns>FALSE if the task fails, TRUE if the task succeeds</returns>
BOOL listProcessInformation() {
	// Take a snapshot of the current every working process(TH32CS_SNAPPROCESS)
	HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
		_tprintf(_T("CreateToolhelp32Snapshot() got an error! Error code: %d\n"), GetLastError());
		return FALSE;
	}

	// A struct object to obtain process information
	// Process32First() retrieves information about the first process encountered in a system snapshot.
	PROCESSENTRY32 processEntry32;
	processEntry32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnapshot, &processEntry32)) {
		_tprintf(_T("Process32First() got an error! Error code: %d\n"), GetLastError());
		CloseHandle(hProcessSnapshot);
		return FALSE;
	}

	do {
		// Print process name and inforamtion
		_tprintf(_T("%40s[PID#%6d]\n"), processEntry32.szExeFile, processEntry32.th32ProcessID);
	} while (Process32Next(hProcessSnapshot, &processEntry32));

	CloseHandle(hProcessSnapshot);
	return TRUE;
}

/// <summary>
///  Custom taskkill(Doing the same thing with taskkill.exe in Windows)
/// </summary>
/// <param name="targetProcessName">Enter the target process name like CalculatorApp.exe or Notepad.exe</param>
/// <returns>FALSE if the task fails, TRUE if the task succeeds</returns>
BOOL terminateProcess(TCHAR* targetProcessName) {
	// Take a snapshot of the current every working process(TH32CS_SNAPPROCESS)
	HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
		_tprintf(_T("CreateToolhelp32Snapshot() got an error! Error code: %d\n"), GetLastError());
		return FALSE;
	}

	// A struct object to obtain process information
	// Process32First() retrieves information about the first process encountered in a system snapshot.
	PROCESSENTRY32 processEntry32;
	processEntry32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnapshot, &processEntry32)) {
		_tprintf(_T("Process32First() got an error! Error code: %d\n"), GetLastError());
		CloseHandle(hProcessSnapshot);
		return FALSE;
	}

	// Search the target process's ID(PID)
	BOOL isTargetProcessFound = FALSE;
	INT targetProcessID = -1;
	do {
		if (!(_tcscmp(StrLower(processEntry32.szExeFile), targetProcessName))) {
			isTargetProcessFound = TRUE;
			targetProcessID = processEntry32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnapshot, &processEntry32));

	if (!isTargetProcessFound) {
		_tprintf(_T("Could not find the target process(%s).\n"), targetProcessName);
		return FALSE;
	}
	else
		_tprintf(_T("Target process(%s) PID found => %d\n"), targetProcessName, targetProcessID);

	// Search the target process handle(HANDLE) based on the process ID
	HANDLE targetProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessID);
	if (targetProcessHandle == NULL) {
		_tprintf(_T("Failed to obtain the target process(%s) Handle\n"), targetProcessName);
		return FALSE;
	}
	else
		_tprintf(_T("Target process(%s) handle found => %ld\n"), targetProcessName, (UINT)targetProcessHandle);

	// Terminate the target process by the obtained HANDLE value
	BOOL isProcessTerminated = TerminateProcess(targetProcessHandle, 0);
	if (!isProcessTerminated) {
		_tprintf(_T("Failed to terminate the process(%s(PID: %d)) with handle(%ld). Got an error code %d\n"),
			targetProcessName, targetProcessID, (UINT)targetProcessHandle, GetLastError());
		return FALSE;
	}
	else
		_tprintf(_T("Target process(%s) successfully termianted.\n"), targetProcessName);

	// finished(successfully)
	return TRUE;

}