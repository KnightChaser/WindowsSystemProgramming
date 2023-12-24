// Windows style string definition
#include <stdio.h>
#include <Windows.h>

int wmain(int argc, wchar_t* argv[]) {

	LPCSTR str1 = "SBCS stylel string 1";		// LPCSTR is a pointer to a const string
	LPCWSTR str2 = L"WBCS style string 1";		// LPCTSTR is a pointer to a const TCHAR string

	CHAR arr1[] = "SBCS style string 2";
	WCHAR arr2[] = L"WBCS style string 2";

	LPCSTR cStr1 = arr1;		// typedef of const char*
	LPCWSTR cStr2 = arr2;		// typedef of const wchar_t*

	printf("%s\n", str1);
	printf("%s\n", arr1);
	wprintf(L"%s\n", str2);
	wprintf(L"%s\n", arr2);

	return 0;
}