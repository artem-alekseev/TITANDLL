#pragma once

#include <Windows.h>


void* basicHook(void* target, void* newFunction, int size = 5, UCHAR hkbyte = 0xE9);
int strposW(wchar_t* haystack, wchar_t* needle);
wchar_t* extractFileNameW(wchar_t* ws);
wchar_t* extractFileDirW(wchar_t* s);
void basicUnhook(void* oldFunc);