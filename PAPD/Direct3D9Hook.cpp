#include "Direct3D9Hook.h"

tDirect3DCreate9 oldDirect3DCreate9 = 0;
std::vector<tNotifyCreateDevice> ncdList;
std::vector<tPresent> presentList;
std::vector<tReset> resetList;

void AddOnNotifyCreateDevice(tNotifyCreateDevice callback)
{
  ncdList.push_back(callback);
}

void AddOnPresent(tPresent callback)
{
  presentList.push_back(callback);
}

void AddOnReset(tReset callback)
{
  resetList.push_back(callback);
}

void D3DCallBack_Present(Direct3DDevice9Wrapper* _this)
{
  for (auto it : presentList)
    it(_this);
}

void D3DCallBack_Reset(Direct3DDevice9Wrapper* _this)
{
  for (auto it : resetList)
    it(_this);
}

void D3DCallBack_NotifyCreateDevice(Direct3D9Wrapper* pD3DWrapper, Direct3DDevice9Wrapper* pD3DDeviceWrapper)
{
  pD3DDeviceWrapper->SetCallBack_Present(D3DCallBack_Present);
  pD3DDeviceWrapper->SetCallBack_Reset(D3DCallBack_Reset);

  basicUnhook(oldDirect3DCreate9);

  for (auto it : ncdList)
    it(pD3DWrapper, pD3DDeviceWrapper);
}

IDirect3D9* __stdcall hkDirect3DCreate9(UINT SDKVersion)
{
  auto pDirect3D9Wrapper = new Direct3D9Wrapper(oldDirect3DCreate9(SDKVersion));

  if (pDirect3D9Wrapper)
    pDirect3D9Wrapper->SetCallBack_NotifyCreateDevice(D3DCallBack_NotifyCreateDevice);

  return pDirect3D9Wrapper;
}

void SetupD3DHook()
{
  if (auto hModule = LoadLibraryA("d3d9.dll"))
  {
    if (auto pAddr = GetProcAddress(hModule, "Direct3DCreate9"))
      oldDirect3DCreate9 = (tDirect3DCreate9)basicHook(pAddr, hkDirect3DCreate9);
  }
}