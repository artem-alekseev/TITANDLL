#include "funcs.h"

void* basicHook(void* target, void* newFunction, int size, UCHAR hkbyte)
{
  void* oldFunc = (void*)VirtualAllocEx(INVALID_HANDLE_VALUE, 0, 10, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  DWORD oldProtect;

  if (oldFunc)
  {
    if (VirtualProtectEx(INVALID_HANDLE_VALUE, target, size, PAGE_EXECUTE_READWRITE, &oldProtect))
    {
      *(UCHAR*)oldFunc = *(UCHAR*)target;
      *(UCHAR*)((UINT_PTR)oldFunc + 5) = 0xE9;
      *(UINT*)((UINT_PTR)oldFunc + 1) = *(UINT*)((UINT_PTR)target + 1);
      *(UINT*)((UINT_PTR)oldFunc + 6) = (UINT)((UINT_PTR)target - (UINT_PTR)oldFunc - 5);

      FillMemory(target, size, 0x90);
      *(UCHAR*)target = hkbyte;
      *(UINT*)((UINT_PTR)target + 1) = (UINT)((UINT_PTR)newFunction - (UINT_PTR)target - 5);
      VirtualProtectEx(INVALID_HANDLE_VALUE, target, size, oldProtect, &oldProtect);
    }
  }

  return oldFunc;
}

void basicUnhook(void* oldFunc)
{
  if (oldFunc)
  {
    void* orgFunc = (void*)(*(UINT_PTR*)((UINT_PTR)oldFunc + 6) + (UINT_PTR)oldFunc + 5);
    DWORD oldProtect;

    if (VirtualProtectEx((HANDLE)-1, orgFunc, 5, PAGE_READWRITE, &oldProtect))
    {
      MoveMemory(orgFunc, oldFunc, 5);
      VirtualProtectEx((HANDLE)-1, orgFunc, 5, oldProtect, &oldProtect);
    }
  }
}

wchar_t* extractFileNameW(wchar_t* ws)
{
  wchar_t wchr = '\\';
  UINT32 i = 0;

  while (ws[i]) i++;                            //find 0
  while ((ws[i - 1] != wchr) && (i != 0)) i--;  //find \

  return &ws[i];
}

int strposW(wchar_t* haystack, wchar_t* needle)
{
  wchar_t* p = wcsstr(haystack, needle);

  if (p)
    return (int)(p - haystack);
  return -1;
}

wchar_t* extractFileDirW(wchar_t* s)
{
  if (s && (strposW(s, (wchar_t*)L"\\") >= 0))
  {
    int i = lstrlenW(s);
    while ((s[i] != '\\') && i)
      s[i--] = 0;
    if (i)
      s[i] = 0;
  }

  return s;
}