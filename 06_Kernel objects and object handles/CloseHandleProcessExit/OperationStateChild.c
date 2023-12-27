// OperationStateChild.c
// Check return value according to the program execution result

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdbool.h>

int _tmain(int argc, TCHAR* argv[]) {

    exit(-1);

	float num1;
	float num2;
	_fputts(_T("Return Value Test...\n"), stdout);
	wscanf_s(_T("%f %f"), &num1, &num2);

	// if (num2 == 0)
		// exit(-1);
		// return -1;

	_tprintf(_T("Operation result: %f\n"), num1 / num2);

	return 1;

}