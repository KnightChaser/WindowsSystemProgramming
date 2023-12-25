// Better solution for polymorphism
// Use WIndows datatype that dynamically changes to the environment

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

/*
#if defined(_WIN64)
	typedef __int64 INT_PTR, * PINT_PTR;
	typedef unsigned __int64 UINT_PTR, * PUINT_PTR;
	// ...
#define __int3264   __int64
#else
	typedef _W64 int INT_PTR, * PINT_PTR;
	typedef _W64 unsigned int UINT_PTR, * PUINT_PTR;
	// ...
#endif
*/

// UINT_PTR will vary either 32bit or 64bit according to the current environment
UINT_PTR calculateDistance(UINT_PTR a, UINT_PTR b) {
	return a - b;
}

int _tmain(int argc, TCHAR* argv[]) {

	INT32 val1 = 10;
	INT32 val2 = 20;

	_tprintf(_T("val1 size: %d\n"), sizeof(val1));
	_tprintf(_T("val2 size: %d\n"), sizeof(val2));
	_tprintf(_T("distance: %d\n"), calculateDistance((UINT_PTR)&val1, (UINT_PTR)&val2));
	
	return 0;
}