// Calculator using process creation in Windows
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdbool.h>

// enumerate for replacing number consts to string
enum {
	DIV = 1,
	MUL,
	ADD,
	MIN,
	ELSE,
	EXIT
};

DWORD ShowMenu();
void Divide(double, double);
void Multiply(double, double);
void Add(double, double);
void Subtract(double, double);

/// <summary>
///  starting point of the calculator with process creation
///  to "calc.exe" additionally
/// </summary>
/// <param name="argc">the number of parameters</param>
/// <param name="argv">the array of parameters</param>
/// <returns>conventional function exit code</returns>
int _tmain(int argc, TCHAR* argv[]) {

	// Basic information for creating process(es)
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);
	TCHAR command[] = _T("calc.exe");
	SetCurrentDirectory(_T("C:\\WINDOWS\\system32"));

	DWORD selection;
	double x, y;
	while (true) {
		selection = ShowMenu();
		if (selection == EXIT) {
			_tprintf(_T("Byebye! OwO *waving*\n"));
			return 0;
		}

		if (selection != ELSE) {
			_fputts(_T("Input two numbers: "), stdout);
			_tscanf(_T("%lf %lf"), &x, &y);
		}

		switch (selection) {
		case DIV:
			Divide(x, y);
			break;
		case MUL:
			Multiply(x, y);
			break;
		case ADD:
			Add(x, y);
			break;
		case MIN:
			Subtract(x, y);
			break;
		case ELSE:
			// similar to memset() with zeroing the designated area
			// If the logic supposed to run "calc.exe", go ahead lol
			ZeroMemory(&pi, sizeof(pi));
			CreateProcess(
				NULL,
				command,
				NULL,
				NULL,
				TRUE,
				0,		// No need to manually create the new console
				NULL,
				NULL,
				&si,
				&pi
			);

			// To prevent the leakage of process handle(process kernel objects),
			// Never forget executing calling CloseHandle() (kernel object usage count)
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}

	return 0;
}

DWORD ShowMenu() {

	DWORD selection;

	_fputts(_T("---------MENU---------\n"), stdout);
	_fputts(_T("#1 DIVISION\n"), stdout);
	_fputts(_T("#2 MULTIPLICATION\n"), stdout);
	_fputts(_T("#3 ADDITION\n"), stdout);
	_fputts(_T("#4 SUBTRACTION\n"), stdout);
	_fputts(_T("#5 ANY OTHER OPERATION?\n"), stdout);
	_fputts(_T("#6 EXIT\n"), stdout);
	_fputts(_T("Enter your expression... >> "), stdout);
	_tscanf(_T("%d"), &selection);

	return selection;
}

void Divide(double a, double b) {
	_tprintf(_T("%f / %f = %f\n\n"), a, b, a / b);
}

void Multiply(double a, double b) {
	_tprintf(_T("%f * %f = %f\n\n"), a, b, a * b);
}

void Add(double a, double b) {
	_tprintf(_T("%f + %f = %f\n\n"), a, b, a + b);
}

void Subtract(double a, double b) {
	_tprintf(_T("%f - %f = %f\n\n"), a, b, a - b);
}