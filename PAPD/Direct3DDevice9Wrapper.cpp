#pragma warning(disable: 26812 26495)
#include "Direct3DDevice9Wrapper.h"

Direct3DDevice9Wrapper::Direct3DDevice9Wrapper(IDirect3DDevice9* pDirect3DDevice9, IDirect3D9* ppDirect3D9, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
  this->pDirect3DDevice9 = pDirect3DDevice9;
  this->pDirect3D9 = ppDirect3D9;

  this->cbBeginScene = 0;
  this->cbEndScene = 0;
  this->cbPresent = 0;
  this->cbReset = 0;
}

HRESULT Direct3DDevice9Wrapper::QueryInterface(REFIID riid, void** ppvObj)
{
 // if ((riid == IID_IDirect3DDevice9 || riid == IID_IUnknown) && ppvObj)
  //{
  //  AddRef();

   // *ppvObj = this;

   // return S_OK;
 // }

  return pDirect3DDevice9->QueryInterface(riid, ppvObj);
}

ULONG Direct3DDevice9Wrapper::AddRef()
{
  return pDirect3DDevice9->AddRef();
}

ULONG Direct3DDevice9Wrapper::Release()
{
  ULONG count = pDirect3DDevice9->Release();

  if (count == 0)
    delete this;

  return count;
}

HRESULT Direct3DDevice9Wrapper::TestCooperativeLevel()
{
  return pDirect3DDevice9->TestCooperativeLevel();
}

UINT Direct3DDevice9Wrapper::GetAvailableTextureMem()
{
  return pDirect3DDevice9->GetAvailableTextureMem();
}

HRESULT Direct3DDevice9Wrapper::EvictManagedResources()
{
  return pDirect3DDevice9->EvictManagedResources();
}

HRESULT Direct3DDevice9Wrapper::GetDirect3D(IDirect3D9** ppD3D9)
{
  if (!ppD3D9)
		return D3DERR_INVALIDCALL;

	if (pDirect3D9)
	{
    pDirect3D9->AddRef();

		*ppD3D9 = pDirect3D9;
	}

	return D3D_OK;
}

HRESULT Direct3DDevice9Wrapper::GetDeviceCaps(D3DCAPS9* pCaps)
{
  return pDirect3DDevice9->GetDeviceCaps(pCaps);
}

HRESULT Direct3DDevice9Wrapper::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
  return pDirect3DDevice9->GetDisplayMode(iSwapChain, pMode);
}

HRESULT Direct3DDevice9Wrapper::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
  return pDirect3DDevice9->GetCreationParameters(pParameters);
}

HRESULT Direct3DDevice9Wrapper::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
  return pDirect3DDevice9->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void Direct3DDevice9Wrapper::SetCursorPosition(int X, int Y, DWORD Flags)
{
  return pDirect3DDevice9->SetCursorPosition(X, Y, Flags);
}

BOOL Direct3DDevice9Wrapper::ShowCursor(BOOL bShow)
{
  return pDirect3DDevice9->ShowCursor(bShow);
}

HRESULT Direct3DDevice9Wrapper::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
  return pDirect3DDevice9->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

HRESULT Direct3DDevice9Wrapper::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
  return pDirect3DDevice9->GetSwapChain(iSwapChain, pSwapChain);
}

UINT Direct3DDevice9Wrapper::GetNumberOfSwapChains()
{
  return pDirect3DDevice9->GetNumberOfSwapChains();
}

HRESULT Direct3DDevice9Wrapper::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
  if (this->cbReset)
    this->cbReset(this);

  return pDirect3DDevice9->Reset(pPresentationParameters);
}

HRESULT Direct3DDevice9Wrapper::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
  if (this->cbPresent)
    this->cbPresent(this);

  return pDirect3DDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT Direct3DDevice9Wrapper::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
  return pDirect3DDevice9->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT Direct3DDevice9Wrapper::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
  return pDirect3DDevice9->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT Direct3DDevice9Wrapper::SetDialogBoxMode(BOOL bEnableDialogs)
{
  return pDirect3DDevice9->SetDialogBoxMode(bEnableDialogs);
}

void Direct3DDevice9Wrapper::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
  return pDirect3DDevice9->SetGammaRamp(iSwapChain, Flags, pRamp);
}

void Direct3DDevice9Wrapper::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
  return pDirect3DDevice9->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT Direct3DDevice9Wrapper::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
{
  return pDirect3DDevice9->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
  return pDirect3DDevice9->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
{
  return pDirect3DDevice9->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
  return pDirect3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
  return pDirect3DDevice9->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
  return pDirect3DDevice9->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
  return pDirect3DDevice9->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
{
  return pDirect3DDevice9->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT Direct3DDevice9Wrapper::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
  return pDirect3DDevice9->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT Direct3DDevice9Wrapper::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
{
  return pDirect3DDevice9->GetRenderTargetData(pRenderTarget, pDestSurface);
}

HRESULT Direct3DDevice9Wrapper::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
  return pDirect3DDevice9->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT Direct3DDevice9Wrapper::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
{
  return pDirect3DDevice9->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT Direct3DDevice9Wrapper::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
  return pDirect3DDevice9->ColorFill(pSurface, pRect, color);
}

HRESULT Direct3DDevice9Wrapper::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
  return pDirect3DDevice9->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT Direct3DDevice9Wrapper::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
  return pDirect3DDevice9->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT Direct3DDevice9Wrapper::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{
  return pDirect3DDevice9->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

HRESULT Direct3DDevice9Wrapper::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
  return pDirect3DDevice9->SetDepthStencilSurface(pNewZStencil);
}

HRESULT Direct3DDevice9Wrapper::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
  return pDirect3DDevice9->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT Direct3DDevice9Wrapper::BeginScene()
{
  if (this->cbBeginScene)
    this->cbBeginScene(this);

  return pDirect3DDevice9->BeginScene();
}

HRESULT Direct3DDevice9Wrapper::EndScene()
{
  if (this->cbEndScene)
    this->cbEndScene(this);

  return pDirect3DDevice9->EndScene();;
}

HRESULT Direct3DDevice9Wrapper::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
  return pDirect3DDevice9->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT Direct3DDevice9Wrapper::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
  return pDirect3DDevice9->SetTransform(State, pMatrix);
}

HRESULT Direct3DDevice9Wrapper::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
  return pDirect3DDevice9->GetTransform(State, pMatrix);
}

HRESULT Direct3DDevice9Wrapper::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
  return pDirect3DDevice9->MultiplyTransform(State, pMatrix);
}

HRESULT Direct3DDevice9Wrapper::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
  return pDirect3DDevice9->SetViewport(pViewport);
}

HRESULT Direct3DDevice9Wrapper::GetViewport(D3DVIEWPORT9* pViewport)
{
  return pDirect3DDevice9->GetViewport(pViewport);
}

HRESULT Direct3DDevice9Wrapper::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
  return pDirect3DDevice9->SetMaterial(pMaterial);
}

HRESULT Direct3DDevice9Wrapper::GetMaterial(D3DMATERIAL9* pMaterial)
{
  return pDirect3DDevice9->GetMaterial(pMaterial);
}

HRESULT Direct3DDevice9Wrapper::SetLight(DWORD Index, CONST D3DLIGHT9* pLight)
{
  return pDirect3DDevice9->SetLight(Index, pLight);
}

HRESULT Direct3DDevice9Wrapper::GetLight(DWORD Index, D3DLIGHT9* pLight)
{
  return pDirect3DDevice9->GetLight(Index, pLight);
}

HRESULT Direct3DDevice9Wrapper::LightEnable(DWORD Index, BOOL Enable)
{
  return pDirect3DDevice9->LightEnable(Index, Enable);
}

HRESULT Direct3DDevice9Wrapper::GetLightEnable(DWORD Index, BOOL* pEnable)
{
  return pDirect3DDevice9->GetLightEnable(Index, pEnable);
}

HRESULT Direct3DDevice9Wrapper::SetClipPlane(DWORD Index, CONST float* pPlane)
{
  return pDirect3DDevice9->SetClipPlane(Index, pPlane);
}

HRESULT Direct3DDevice9Wrapper::GetClipPlane(DWORD Index, float* pPlane)
{
  return pDirect3DDevice9->GetClipPlane(Index, pPlane);
}

HRESULT Direct3DDevice9Wrapper::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
  return pDirect3DDevice9->SetRenderState(State, Value);
}

HRESULT Direct3DDevice9Wrapper::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
  return pDirect3DDevice9->GetRenderState(State, pValue);
}

HRESULT Direct3DDevice9Wrapper::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
  return pDirect3DDevice9->CreateStateBlock(Type, ppSB);
}

HRESULT Direct3DDevice9Wrapper::BeginStateBlock()
{
  return pDirect3DDevice9->BeginStateBlock();
}

HRESULT Direct3DDevice9Wrapper::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
  return pDirect3DDevice9->EndStateBlock(ppSB);
}

HRESULT Direct3DDevice9Wrapper::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
  return pDirect3DDevice9->SetClipStatus(pClipStatus);
}

HRESULT Direct3DDevice9Wrapper::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
  return pDirect3DDevice9->GetClipStatus(pClipStatus);
}

HRESULT Direct3DDevice9Wrapper::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
  return pDirect3DDevice9->GetTexture(Stage, ppTexture);
}

HRESULT Direct3DDevice9Wrapper::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
{
  return pDirect3DDevice9->SetTexture(Stage, pTexture);
}

HRESULT Direct3DDevice9Wrapper::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
  return pDirect3DDevice9->GetTextureStageState(Stage, Type, pValue);
}

HRESULT Direct3DDevice9Wrapper::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
  return pDirect3DDevice9->SetTextureStageState(Stage, Type, Value);
}

HRESULT Direct3DDevice9Wrapper::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
  return pDirect3DDevice9->GetSamplerState(Sampler, Type, pValue);
}

HRESULT Direct3DDevice9Wrapper::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
  return pDirect3DDevice9->SetSamplerState(Sampler, Type, Value);
}

HRESULT Direct3DDevice9Wrapper::ValidateDevice(DWORD* pNumPasses)
{
  return pDirect3DDevice9->ValidateDevice(pNumPasses);
}

HRESULT Direct3DDevice9Wrapper::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
  return pDirect3DDevice9->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT Direct3DDevice9Wrapper::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
{
  return pDirect3DDevice9->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT Direct3DDevice9Wrapper::SetCurrentTexturePalette(UINT PaletteNumber)
{
  return pDirect3DDevice9->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT Direct3DDevice9Wrapper::GetCurrentTexturePalette(UINT *PaletteNumber)
{
  return pDirect3DDevice9->GetCurrentTexturePalette(PaletteNumber);
}

HRESULT Direct3DDevice9Wrapper::SetScissorRect(CONST RECT* pRect)
{
  return pDirect3DDevice9->SetScissorRect(pRect);
}

HRESULT Direct3DDevice9Wrapper::GetScissorRect(RECT* pRect)
{
  return pDirect3DDevice9->GetScissorRect(pRect);
}

HRESULT Direct3DDevice9Wrapper::SetSoftwareVertexProcessing(BOOL bSoftware)
{
  return pDirect3DDevice9->SetSoftwareVertexProcessing(bSoftware);
}

BOOL Direct3DDevice9Wrapper::GetSoftwareVertexProcessing()
{
  return pDirect3DDevice9->GetSoftwareVertexProcessing();
}

HRESULT Direct3DDevice9Wrapper::SetNPatchMode(float nSegments)
{
  return pDirect3DDevice9->SetNPatchMode(nSegments);
}

float Direct3DDevice9Wrapper::GetNPatchMode()
{
  return pDirect3DDevice9->GetNPatchMode();
}

HRESULT Direct3DDevice9Wrapper::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
  return pDirect3DDevice9->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT Direct3DDevice9Wrapper::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
  return pDirect3DDevice9->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT Direct3DDevice9Wrapper::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
  return pDirect3DDevice9->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT Direct3DDevice9Wrapper::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
  return pDirect3DDevice9->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT Direct3DDevice9Wrapper::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
  return pDirect3DDevice9->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

HRESULT Direct3DDevice9Wrapper::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
  return pDirect3DDevice9->CreateVertexDeclaration(pVertexElements, ppDecl);
}

HRESULT Direct3DDevice9Wrapper::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
  return pDirect3DDevice9->SetVertexDeclaration(pDecl);
}

HRESULT Direct3DDevice9Wrapper::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
  return pDirect3DDevice9->GetVertexDeclaration(ppDecl);
}

HRESULT Direct3DDevice9Wrapper::SetFVF(DWORD FVF)
{
  return pDirect3DDevice9->SetFVF(FVF);
}

HRESULT Direct3DDevice9Wrapper::GetFVF(DWORD* pFVF)
{
  return pDirect3DDevice9->GetFVF(pFVF);
}

HRESULT Direct3DDevice9Wrapper::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
  return pDirect3DDevice9->CreateVertexShader(pFunction, ppShader);
}

HRESULT Direct3DDevice9Wrapper::SetVertexShader(IDirect3DVertexShader9* pShader)
{
  return pDirect3DDevice9->SetVertexShader(pShader);
}

HRESULT Direct3DDevice9Wrapper::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
  return pDirect3DDevice9->GetVertexShader(ppShader);
}

HRESULT Direct3DDevice9Wrapper::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
  return pDirect3DDevice9->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT Direct3DDevice9Wrapper::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
  return pDirect3DDevice9->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT Direct3DDevice9Wrapper::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
  return pDirect3DDevice9->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT Direct3DDevice9Wrapper::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
  return pDirect3DDevice9->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT Direct3DDevice9Wrapper::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
  return pDirect3DDevice9->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT Direct3DDevice9Wrapper::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
  return pDirect3DDevice9->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT Direct3DDevice9Wrapper::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
  return pDirect3DDevice9->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT Direct3DDevice9Wrapper::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride)
{
  return pDirect3DDevice9->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

HRESULT Direct3DDevice9Wrapper::SetStreamSourceFreq(UINT StreamNumber, UINT Setting)
{
  return pDirect3DDevice9->SetStreamSourceFreq(StreamNumber, Setting);
}

HRESULT Direct3DDevice9Wrapper::GetStreamSourceFreq(UINT StreamNumber, UINT* pSetting)
{
  return pDirect3DDevice9->GetStreamSourceFreq(StreamNumber, pSetting);
}

HRESULT Direct3DDevice9Wrapper::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
  return pDirect3DDevice9->SetIndices(pIndexData);
}

HRESULT Direct3DDevice9Wrapper::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
  return pDirect3DDevice9->GetIndices(ppIndexData);
}

HRESULT Direct3DDevice9Wrapper::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
  return pDirect3DDevice9->CreatePixelShader(pFunction, ppShader);
}

HRESULT Direct3DDevice9Wrapper::SetPixelShader(IDirect3DPixelShader9* pShader)
{
  return pDirect3DDevice9->SetPixelShader(pShader);
}

HRESULT Direct3DDevice9Wrapper::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
  return pDirect3DDevice9->GetPixelShader(ppShader);
}

HRESULT Direct3DDevice9Wrapper::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
  return pDirect3DDevice9->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT Direct3DDevice9Wrapper::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
  return pDirect3DDevice9->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT Direct3DDevice9Wrapper::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
  return pDirect3DDevice9->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT Direct3DDevice9Wrapper::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
  return pDirect3DDevice9->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT Direct3DDevice9Wrapper::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
  return pDirect3DDevice9->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT Direct3DDevice9Wrapper::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
  return pDirect3DDevice9->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT Direct3DDevice9Wrapper::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
  return pDirect3DDevice9->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT Direct3DDevice9Wrapper::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
  return pDirect3DDevice9->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT Direct3DDevice9Wrapper::DeletePatch(UINT Handle)
{
  return pDirect3DDevice9->DeletePatch(Handle);
}

HRESULT Direct3DDevice9Wrapper::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
  return pDirect3DDevice9->CreateQuery(Type, ppQuery);
}