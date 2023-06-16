#include "Direct3D9Hook.h"
#include <d3dx9tex.h>

void CheckTexture(Direct3DDevice9Wrapper* pD3DDeviceWrapper, LPDIRECT3DTEXTURE9& texture, LPCWSTR path);
void CheckFont(Direct3DDevice9Wrapper* pD3DDeviceWrapper, int fontSize, LPCWSTR fontStyle, LPD3DXFONT& font);
void DrawTexture(LPD3DXSPRITE sprite, D3DXVECTOR2 pos, LPDIRECT3DTEXTURE9& texture, UINT8 alpha);
void CheckSprite(Direct3DDevice9Wrapper* pD3DDeviceWrapper, LPD3DXSPRITE& sprite);
void ReleaseElement(LPUNKNOWN& element);