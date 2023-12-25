// Polymorphic Type

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

// Not so clean example for polymorphism
#define _WIN64

#if defined(_WIN64)
	UINT64 calculateDistance(UINT64 a, UINT64 b)
#else
	UINT calculateDistance(UINT a, UINT b)
#endif
{
	return a - b;
}

int _tmain(int argc, TCHAR* argv[]) {
	INT val1 = 10;
	INT val2 = 20;
	_tprintf(_T("Position: 0x%x, 0x%x\n"), (UINT)&val1, (UINT)&val2);
#if defined(_WIN64)
	_tprintf(_T("Distance: %lld\n"), calculateDistance((UINT64)&val1, (UINT64)&val2));
#else
	_tprintf(_T("Distance: %u\n"), calculateDistance((UINT)&val1, (UINT)&val2));
#endif

	return 0;
}