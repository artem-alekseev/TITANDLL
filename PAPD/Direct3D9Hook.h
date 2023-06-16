#pragma once

#include "Direct3DDevice9Wrapper.h"
#include "Direct3D9Wrapper.h"

#include "funcs.h"

#include <vector>

using tNotifyCreateDevice = void(*)(Direct3D9Wrapper* pD3DWrapper, Direct3DDevice9Wrapper* pD3DDeviceWrapper);
using tDirect3DCreate9 = IDirect3D9 * (__stdcall*)(UINT SDKVersion);
using tPresent = void(*)(Direct3DDevice9Wrapper* _this);
using tReset = tPresent;

void AddOnNotifyCreateDevice(tNotifyCreateDevice callback);
void AddOnPresent(tPresent callback);
void AddOnReset(tReset callback);
void SetupD3DHook();