#pragma once
#include <Windows.h>

template <typename T>
struct RetType
{
	typedef T(__cdecl* t_cdecl)();
	typedef T(__stdcall* t_stdcall)();
	typedef T(__thiscall* t_thiscall)(void*);
};

typedef struct _SHOOK
{

}SHOOK;


class Hook
{
public:
	virtual ~Hook();

	static Hook* GetInstance()
	{
		static Hook instance;
		return &instance;
	}

	BOOL Init();

	BOOL SetHook(DWORD dwSrc, DWORD dwDest, BYTE OpCodeHook);
	BOOL SetHook(DWORD dwSrc, DWORD dwDest, BYTE OpCodeHook, UINT uOpCodeLength, PVOID& pMain, BOOL bState);


private:
	Hook(CONST Hook&);
	Hook operator=(CONST Hook&);
	Hook();

	DWORD CalcDist(DWORD dwSrc, DWORD dwDest);
};

