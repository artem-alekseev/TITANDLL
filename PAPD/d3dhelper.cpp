#include "d3dhelper.h"

void CheckTexture(Direct3DDevice9Wrapper* pD3DDeviceWrapper, LPDIRECT3DTEXTURE9& texture, LPCWSTR path)
{
  if (!texture)
    D3DXCreateTextureFromFileW(pD3DDeviceWrapper, path, &texture);
}

void CheckFont(Direct3DDevice9Wrapper* pD3DDeviceWrapper, int fontSize, LPCWSTR fontStyle, LPD3DXFONT& font)
{
    if (!font)
        D3DXCreateFontW(pD3DDeviceWrapper, fontSize, NULL, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontStyle, &font);
}

void CheckSprite(Direct3DDevice9Wrapper* pD3DDeviceWrapper, LPD3DXSPRITE& sprite)
{
  if (!sprite)
    D3DXCreateSprite(pD3DDeviceWrapper, &sprite);
}

void ReleaseElement(LPUNKNOWN& element)
{
  if (element)
  {
    element->Release();
    element = 0;
  }
}

void DrawTexture(LPD3DXSPRITE sprite, D3DXVECTOR2 pos, LPDIRECT3DTEXTURE9& texture, UINT8 alpha)
{
  D3DXVECTOR3 pos3{ pos.x, pos.y, 0};

  sprite->Draw(texture, 0, 0, &pos3, D3DCOLOR_RGBA(255, 255, 255, alpha));
}