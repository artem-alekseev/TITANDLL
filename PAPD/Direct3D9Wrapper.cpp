#pragma warning(disable: 26812 26495)
#include "Direct3D9Wrapper.h"

HRESULT Direct3D9Wrapper::QueryInterface(const IID &riid, void **ppvObj)
{
  /*if ((riid == IID_IDirect3D9 || riid == IID_IUnknown) && ppvObj)
  {
    AddRef();

    *ppvObj = this;

    return S_OK;
  }*/

  return pDirect3D9->QueryInterface(riid, ppvObj);
}

ULONG Direct3D9Wrapper::AddRef()
{
  return pDirect3D9->AddRef();
}

ULONG Direct3D9Wrapper::Release()
{
  ULONG count = pDirect3D9->Release();

  if (count == 0)
    delete this;

  return count;
}

HRESULT  Direct3D9Wrapper::RegisterSoftwareDevice(void* pInitializeFunction)
{
  return pDirect3D9->RegisterSoftwareDevice(pInitializeFunction);
}

UINT Direct3D9Wrapper::GetAdapterCount()
{
  return pDirect3D9->GetAdapterCount();
}

HRESULT Direct3D9Wrapper::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier)
{
  return pDirect3D9->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT Direct3D9Wrapper::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
{
  return pDirect3D9->GetAdapterModeCount(Adapter, Format);
}

HRESULT Direct3D9Wrapper::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode)
{
  return pDirect3D9->EnumAdapterModes(Adapter, Format, Mode, pMode);
}

HRESULT Direct3D9Wrapper::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode)
{
  return pDirect3D9->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT Direct3D9Wrapper::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
  return pDirect3D9->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
}

HRESULT Direct3D9Wrapper::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
  return pDirect3D9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT Direct3D9Wrapper::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)
{
  return pDirect3D9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

HRESULT Direct3D9Wrapper::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
  return pDirect3D9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT Direct3D9Wrapper::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
  return pDirect3D9->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
}

HRESULT Direct3D9Wrapper::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
{
  return pDirect3D9->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HMONITOR Direct3D9Wrapper::GetAdapterMonitor(UINT Adapter)
{
  return pDirect3D9->GetAdapterMonitor(Adapter);
}

HRESULT Direct3D9Wrapper::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
  HRESULT result = pDirect3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

	if (SUCCEEDED(result) && ppReturnedDeviceInterface)
    *ppReturnedDeviceInterface = new Direct3DDevice9Wrapper(*ppReturnedDeviceInterface, this, pPresentationParameters);

  if (this->cbNotifyCreateDevice)
  {
    Direct3DDevice9Wrapper* ptr = 0;

    if (ppReturnedDeviceInterface)
      ptr = (Direct3DDevice9Wrapper*)(*ppReturnedDeviceInterface);

    this->cbNotifyCreateDevice(this, ptr);
  }

  return result;
}