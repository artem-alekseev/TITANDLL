#include "Hook.h"


Hook::Hook()
{
}


Hook::~Hook()
{
}

BOOL Hook::Init()
{
	return TRUE;
}

BOOL Hook::SetHook(DWORD dwSrc, DWORD dwDest, BYTE OpCodeHook)
{
	DWORD dwOld;

	if (!VirtualProtect((PVOID)dwSrc, 0x40, PAGE_EXECUTE_READWRITE, &dwOld)) return FALSE;

	*(LPBYTE)((LPBYTE)dwSrc) = OpCodeHook;
	*(PULONG)((LPBYTE)dwSrc + 1) = CalcDist(dwSrc, dwDest);

	VirtualProtect((PVOID)dwSrc, 0x40, dwOld, &dwOld);

	return TRUE;
}

BOOL Hook::SetHook(DWORD dwSrc, DWORD dwDest, BYTE OpCodeHook, UINT uOpCodeLength, PVOID& pMain, BOOL bState)
{
	DWORD dwOld;

	if (!VirtualProtect((PVOID)dwSrc, 0x40, PAGE_EXECUTE_READWRITE, &dwOld)) return FALSE;

	if (!bState)
	{
		pMain = VirtualAlloc(NULL, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		if (pMain == NULL) return FALSE;

		CopyMemory(pMain, (PVOID)dwSrc, uOpCodeLength);
		*(LPBYTE)((LPBYTE)pMain + uOpCodeLength + 1) = OpCodeHook;
		*(PULONG)((LPBYTE)pMain + uOpCodeLength + 2) = CalcDist((DWORD)pMain + uOpCodeLength, dwSrc + uOpCodeLength);
	}
	else
		pMain = (PVOID)(*(PULONG)(dwSrc + 1) + dwSrc + 5);

	*(LPBYTE)((LPBYTE)dwSrc) = OpCodeHook;
	*(PULONG)((LPBYTE)dwSrc + 1) = CalcDist(dwSrc, dwDest);

	VirtualProtect((PVOID)dwSrc, 0x40, dwOld, &dwOld);

	return TRUE;
}

DWORD Hook::CalcDist(DWORD dwSrc, DWORD dwDest)
{
	DWORD Result;

	if (dwDest < dwSrc)
	{
		Result = dwSrc - dwDest;
		Result = 0xFFFFFFFF - Result;
		Result -= 4;
	}
	else
	{
		Result = dwDest - dwSrc;
		Result -= 5;
	}

	return Result;
}