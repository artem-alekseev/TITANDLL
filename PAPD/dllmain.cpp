#include <windows.h>
#include "Direct3D9Hook.h"
#include "d3dhelper.h"
#include "ColorNames.h"
#include "VMProtectSDK.h"
#include "Hook.h"
#include <sstream>
#include <d3d9.h>
#include <d3dx9.h>
#include <detours.h>
#include <map>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "VMProtectSDK32.lib")

typedef void(WINAPI* tCreateCharacter)();
typedef void(WINAPI* tExitCreateCharacter)();
typedef void(WINAPI* tExitAfterCreateCharacter)();
typedef void(WINAPI* tAnimationCreateCharacter)(DWORD* className);
tCreateCharacter oCreateCharacter;
tExitCreateCharacter oExitCreateCharacter;
tExitAfterCreateCharacter oExitAfterCreateCharacter;
tAnimationCreateCharacter oAnimationCreateCharacter;

void DrawTextureClassChange(RECT imagePosInDds, D3DXVECTOR3 Position);
void DrawClassTexture(RECT imagePosInDds, D3DXVECTOR3 Position);
void DrawTextureBaldus(RECT imagePosInDds, D3DXVECTOR3 Position);
void DrawDMTexture(RECT imagePosInDds, D3DXVECTOR3 Position);
void DrawHightTextWithoutOffset(RECT TextPos, char text[]);
void DrawNameTexture(RECT imagePosInDds, D3DXVECTOR3 Position);
void DrawTexture(RECT imagePosInDds, D3DXVECTOR3 Position);
void DrawTextWithOffset(RECT TextPos, char text[]);
void DrawTextWithoutOffsetGrey(RECT TextPos, char text[]);
void DrawTextWithoutOffset(RECT TextPos, char text[]);
void DrawMouse(LPDIRECT3DDEVICE9 pDevice);
void Patch(DWORD dst, BYTE* src, const intptr_t size);
bool CheckClick(D3DXVECTOR3 classPos, RECT imagePosInDds);
void Activate(BYTE classByte);

bool ViewClass = false;

tagPOINT CursorPos;

RECT imagePosWAInDds = { 0 , 296 , 281 , 354 };
RECT imagePosBLInDds = { 0 , 365 , 281 , 423 };
RECT imagePosGLInDds = { 0 , 434 , 281 , 492 };
RECT imagePosFBInDds = { 0 , 503 , 281 , 561 };
RECT imagePosFSInDds = { 0 , 571 , 281 , 629 };

RECT imagePosFAInDds = { 0 , 640 , 281 , 698 };
RECT imagePosWIInDds = { 0 , 709 , 281 , 767 };
RECT imagePosFGInDds = { 0 , 778 , 281 , 836 };
RECT imagePosDMInDds = { 12 , 676 , 295 , 734 };

RECT imagePosWAActiveInDds = { 292 , 296 , 573 , 354 };
RECT imagePosBLActiveInDds = { 292 , 365 , 573 , 423 };
RECT imagePosGLActiveInDds = { 292 , 434 , 573 , 492 };
RECT imagePosFBActiveInDds = { 292 , 503 , 573 , 561 };
RECT imagePosFSActiveInDds = { 292 , 571 , 573 , 629 };

RECT imagePosFAActiveInDds = { 292 , 640 , 573 , 698 };
RECT imagePosWIActiveInDds = { 584 , 296 , 865 , 354 };
RECT imagePosFGActiveInDds = { 584 , 365 , 865 , 423 };
RECT imagePosDMActiveInDds = { 13 , 745 , 296 , 803 };

RECT imagePosActiveTabInDds = { 0 , 245 , 125 , 286 };
RECT imagePosDisactiveTabInDds = { 135 , 245 , 260 , 286 };

D3DXVECTOR3 classWAPos = { float(0), float(46), 0.0f };
D3DXVECTOR3 classBLPos = { float(0), float(115), 0.0f };
D3DXVECTOR3 classGLPos = { float(0), float(184), 0.0f };
D3DXVECTOR3 classFBPos = { float(0), float(253), 0.0f };
D3DXVECTOR3 classFSPos = { float(0), float(321), 0.0f };

D3DXVECTOR3 classFAPos = { float(0), float(46), 0.0f };
D3DXVECTOR3 classWIPos = { float(0), float(115), 0.0f };
D3DXVECTOR3 classFGPos = { float(0), float(184), 0.0f };
D3DXVECTOR3 classDMPos = { float(0), float(253), 0.0f };

D3DXVECTOR3 TabSWPos = { float(0), float(0), 0.0f };
D3DXVECTOR3 TabMAPos = { float(155), float(0), 0.0f };

RECT TabSWTextPos = { 0, 0, 125, 41 };
RECT TabMATextPos = { 155, 0, 280, 41 };

RECT imageNamePosInDds = { 564, 673, 943, 772 };
D3DXVECTOR3 NamePosition = { float(-81), float(-170), 0.0f };
RECT NameTextPossition = { -81, -150, 298, -130 };
RECT NameTitleTextPossition = { 120, 60, 345, 110 };
RECT NameDescriptionTextPossition = { 120, 90, 645, 130 };
RECT NameFullDescriptionTextPossition = { 45, 130, 550, 230 };
RECT NameWeaponPossition = { 45, 240, 120, 340 };
RECT NameWeaponTextPossition = { 120, 240, 450, 340 };

RECT NameStatsStrPossition = { 45, 260, 120, 360 };
RECT NameStatsStrTextPossition = { 120, 260, 450, 360 };

RECT NameStatsIntPossition = { 45, 280, 120, 380 };
RECT NameStatsIntTextPossition = { 120, 280, 450, 380 };

RECT NameStatsDexPossition = { 45, 300, 120, 400 };
RECT NameStatsDexTextPossition = { 120, 300, 450, 400 };

RECT classWATextPos = { 0, 46, 200, 104 };
RECT classBLTextPos = { 0, 115, 200, 173 };
RECT classGLTextPos = { 0, 184, 200, 242 };
RECT classFBTextPos = { 0, 253, 200, 311 };
RECT classFSTextPos = { 0, 321, 200, 379 };

RECT classFATextPos = { 0, 46, 200, 104 };
RECT classWITextPos = { 0, 115, 200, 173 };
RECT classFGTextPos = { 0, 184, 200, 242 };
RECT classDMTextPos = { 0, 253, 200, 311 };

D3DXVECTOR3 CurrentCharacterNamePosition = { float(35), float(50), 0.0f };
RECT CurrentCharacterNameWAInDds = { 935 , 107 , 1000 , 171 };
RECT CurrentCharacterNameBLInDds = { 935 , 179 , 1000 , 243 };
RECT CurrentCharacterNameGLInDds = { 935 , 611 , 1000 , 675 };
RECT CurrentCharacterNameFBInDds = { 935 , 465 , 1000 , 529 };
RECT CurrentCharacterNameFSInDds = { 935 , 321 , 1000 , 385 };
RECT CurrentCharacterNameFAInDds = { 935 , 393 , 1000 , 457 };
RECT CurrentCharacterNameWIInDds = { 935 , 250 , 1000 , 314 };
RECT CurrentCharacterNameFGInDds = { 935 , 539 , 1000 , 603 };
RECT CurrentCharacterNameDMInDds = { 935 , 752 , 1000 , 816 };

const BYTE WA = 0x01;
const BYTE BL = 0x02;
const BYTE GL = 0x03;
const BYTE FB = 0x04;
const BYTE FS = 0x05;
const BYTE FA = 0x06;
const BYTE WI = 0x07;
const BYTE FG = 0x08;
const BYTE DM = 0x09;

DWORD WAANIE = 0x31;
DWORD BLANIE = 0x32;
DWORD GLANIE = 0x31;
DWORD FBANIE = 0x36;
DWORD FSANIE = 0x35;
DWORD FAANIE = 0x34;
DWORD WIANIE = 0x33;
DWORD FGANIE = 0x34;
DWORD DMANIE = 0x33;

BYTE CurrentClass = WA;

const BYTE TabSW = 0x01;
const BYTE TabMA = 0x02;

BYTE ActiveClassTab = TabSW;

BYTE WA_ITEMS[20] = {
    0xCF, 0x03, 0x00, 0x00,
    0xD5, 0x03, 0x00, 0x00,
    0xDB, 0x03, 0x00, 0x00,
    0x66, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

BYTE GL_ITEMS[20] = {
    0xCF, 0x03, 0x00, 0x00,
    0xD5, 0x03, 0x00, 0x00,
    0xDB, 0x03, 0x00, 0x00,
    0xAF, 0x0F, 0x00, 0x00,
    0xAF, 0x0F, 0x00, 0x00,
};

BYTE WI_ITEMS[20] = {
    0xD4, 0x03, 0x00, 0x00,
    0xDA, 0x03, 0x00, 0x00,
    0xE0, 0x03, 0x00, 0x00,
    0x1B, 0x00, 0x00, 0x00,
    0x1B, 0x00, 0x00, 0x00,
};

BYTE DM_ITEMS[20] = {
    0x5A, 0x09, 0x00, 0x00,//item2394
    0xDA, 0x03, 0x00, 0x00,
    0xE0, 0x03, 0x00, 0x00,
    0xA9, 0x04, 0x00, 0x00,
    0xA8, 0x04, 0x00, 0x00,
};

LPD3DXSPRITE SptMenu;
LPDIRECT3DTEXTURE9 PAPDTexture;
LPDIRECT3DTEXTURE9 BaldusTexture;
LPDIRECT3DTEXTURE9 RagePointsTexture;
LPDIRECT3DTEXTURE9 ClassTexture;
LPDIRECT3DTEXTURE9 DMClassTexture;
LPDIRECT3DTEXTURE9 EnterNameTexture;

LPD3DXFONT miniFont;
LPD3DXFONT font;
LPD3DXFONT hightFont;

int MenuX = 100;    ///Starting 
int MenuY = 75;    ///Positions
int xDifference = 0;
int yDifference = 0;

bool isMenuMoving = false;

RECT imageSize = { 0, 0, 120, 20 };

DWORD PGlobal = 0x00B93530;
DWORD MAP_IDS = 0xBA0760;
DWORD LOGIN_ON_AND_OFF123 = 0x4810;
DWORD CURRENT_CLASS = 0x41A8;
DWORD CURRENT_BM_STATE = 0x41B0;

DWORD Atribut_HONRA = 0x4760;

DWORD Atribut_FOR = 0x6D80;
DWORD Atribut_INT = 0x6D84;
DWORD Atribut_AMP_E = 0x6D94;
DWORD Atribut_AMP_M = 0x6D98;
DWORD Atribut_ATTACK = 0x49CC;
DWORD Atribut_ATTACK_M = 0x49D0;
DWORD Atribut_ATTACK_ALL = 0x6D9C;
DWORD Atribut_AMP_ALL = 0x6DA0;
DWORD Atribut_DANO_ADC = 0x6DB8;
DWORD Atribut_ABC_HP_MP = 0x6DBC;
DWORD Atribut_LMT_ABS_HP = 0x6DC4;
DWORD Atribut_LMT_ABS_MP = 0x6DC8;
DWORD Atribut_DANO_MIN = 0x6DD4;
DWORD Atribut_DC = 0x6DD8;
DWORD Atribut_TAXA = 0x6DDC;
DWORD Atribut_MAX_TAXA = 0x6DE0;
DWORD Atribut_PREC = 0x2835;

DWORD Atribut_DES = 0x6D88;
DWORD Atribut_HP = 0x6D8C;
DWORD Atribut_MP = 0x6D90;
DWORD Atribut_RESS_TAXA = 0x6DA8;
DWORD Atribut_RESS_DC = 0x6DAC;
DWORD Atribut_RESS_AMP = 0x6DB0;
DWORD Atribut_ESQUIVA = 0x6DB4;
DWORD Atribut_AUT_CURA_HP = 0x6DCC;
DWORD Atribut_AUT_CURA_MP = 0x6DD0;
DWORD Atribut_DEF = 0x49D4;
DWORD Atribut_EVA = 0x6E14;

DWORD XPoints = 0;
DWORD RagePoints = 0;

void CheckAllTextures(Direct3DDevice9Wrapper* _this)
{
    CheckSprite(_this, SptMenu);
    CheckTexture(_this, PAPDTexture, L"Data/UI/PAPD.png");
    CheckTexture(_this, BaldusTexture, L"Data/UI/Theme1/Baldus/Baldus.png");
    CheckTexture(_this, RagePointsTexture, L"Data/UI/Theme1/ui_texture17.dds");
    CheckTexture(_this, ClassTexture, L"Data/UI/Theme1/UI_Texture25.dds");
    CheckTexture(_this, DMClassTexture, L"Data/UI/Theme1/UI_Texture55.dds");
    CheckTexture(_this, EnterNameTexture, L"Data/UI/Theme1/UI_Texture26.dds");

    CheckFont(_this, 12, L"Verdana", miniFont);
    CheckFont(_this, 16, L"Verdana", font);
    CheckFont(_this, 32, L"Verdana", hightFont);
}

void NotifyCreateDevice(Direct3D9Wrapper* pD3DWrapper, Direct3DDevice9Wrapper* pD3DDeviceWrapper)
{
    CheckAllTextures(pD3DDeviceWrapper);
}

void DrawTextWithoutOffsetColor(RECT TextPos, char text[], D3DCOLOR color)
{
    miniFont->DrawTextA(SptMenu, text, -1, &TextPos, DT_CENTER, color);
}

void DrawTextureWithoutOffset(RECT imagePosInDds, D3DXVECTOR3 Position)
{
    SptMenu->Draw(PAPDTexture, &imagePosInDds, NULL, &Position, 0xFFFFFFFF);
}

void DrawTextureXPoints(D3DXVECTOR3 Position)
{
    RECT imagePosInDds = { 678 , 2 , 855 , 6 };

    imagePosInDds.right = imagePosInDds.right - 177 + (XPoints);

    SptMenu->Draw(RagePointsTexture, &imagePosInDds, NULL, &Position, 0xFFFFFFFF);
}

void DrawTextureRagePoints(D3DXVECTOR3 Position)
{
    RECT imagePosInDdsMain = { 528 , 293 , 592 , 357 };
    SptMenu->Draw(RagePointsTexture, &imagePosInDdsMain, NULL, &Position, 0xFFFFFFFF);

    if (RagePoints == 0)
        return;
    RECT imagePosInDdsPices1 = { 595 , 328 , 620 , 360 };
    D3DXVECTOR3 Position1 = Position;
    Position1.x += 22;
    Position1.y += 3;
    SptMenu->Draw(RagePointsTexture, &imagePosInDdsPices1, NULL, &Position1, 0xFFFFFFFF);
    
    if (RagePoints == 1)
        return;
    RECT imagePosInDdsPices2 = { 621 , 328 , 645 , 348 };
    D3DXVECTOR3 Position2 = Position;
    Position2.x += 40;
    Position2.y += 6;
    SptMenu->Draw(RagePointsTexture, &imagePosInDdsPices2, NULL, &Position2, 0xFFFFFFFF);

    if (RagePoints == 2)
        return;
    RECT imagePosInDdsPices3 = { 644 , 328 , 668 , 348 };
    D3DXVECTOR3 Position3 = Position;
    Position3.x += 45;
    Position3.y += 16;
    SptMenu->Draw(RagePointsTexture, &imagePosInDdsPices3, NULL, &Position3, 0xFFFFFFFF);

    if (RagePoints == 3)
        return;
    RECT imagePosInDdsPices4 = { 668 , 328 , 692 , 348 };
    D3DXVECTOR3 Position4 = Position;
    Position4.x += 45;
    Position4.y += 29;
    SptMenu->Draw(RagePointsTexture, &imagePosInDdsPices4, NULL, &Position4, 0xFFFFFFFF);

    if (RagePoints == 4)
        return;
    RECT imagePosInDdsPices5 = { 692 , 328 , 716, 348 };
    D3DXVECTOR3 Position5 = Position;
    Position5.x += 38;
    Position5.y += 40;
    SptMenu->Draw(RagePointsTexture, &imagePosInDdsPices5, NULL, &Position5, 0xFFFFFFFF);

    if (RagePoints == 5)
        return;
    RECT imagePosInDdsPices6 = { 716 , 326 , 740 , 348 };
    D3DXVECTOR3 Position6 = Position;
    Position6.x += 24;
    Position6.y += 42;
    SptMenu->Draw(RagePointsTexture, &imagePosInDdsPices6, NULL, &Position6, 0xFFFFFFFF);

    if (RagePoints == 6)
        return;
    RECT imagePosInDdsPices7 = { 740 , 328 , 764 , 348 };
    D3DXVECTOR3 Position7 = Position;
    Position7.x += 13;
    Position7.y += 40;
    SptMenu->Draw(RagePointsTexture, &imagePosInDdsPices7, NULL, &Position7, 0xFFFFFFFF);

    if (RagePoints == 7)
        return;
    RECT imagePosInDdsPices8 = { 762 , 328 , 786 , 348 };
    D3DXVECTOR3 Position8 = Position;
    Position8.x += 6;
    Position8.y += 30;
    SptMenu->Draw(RagePointsTexture, &imagePosInDdsPices8, NULL, &Position8, 0xFFFFFFFF);

    if (RagePoints == 8)
        return;
    RECT imagePosInDdsPices9 = { 786 , 328 , 808 , 348 };
    D3DXVECTOR3 Position9 = Position;
    Position9.x += 3;
    Position9.y += 15;
    SptMenu->Draw(RagePointsTexture, &imagePosInDdsPices9, NULL, &Position9, 0xFFFFFFFF);

    if (RagePoints == 9)
        return;
    RECT imagePosInDdsPices10 = { 808 , 328 , 832 , 348 };
    D3DXVECTOR3 Position10 = Position;
    Position10.x += 5;
    Position10.y += 6;
    SptMenu->Draw(RagePointsTexture, &imagePosInDdsPices10, NULL, &Position10, 0xFFFFFFFF);
}

__declspec(noinline) int GetClassId(int Style)
{
	if ((Style & 0xF0000000) == 0x20000000)
		return 7;

	if ((Style & 0xF0000000) == 0x30000000)
		return 8;

	if ((Style & 0xF0000000) == 0x40000000)
		return 9;

	return Style & 7;
}

DWORD isNpcOpen = 0xBA0754;
D3DVIEWPORT9 Viewport;

void Present(Direct3DDevice9Wrapper* _this)
{
    _this->GetViewport(&Viewport);

    CheckAllTextures(_this);

    SptMenu->Begin(D3DXSPRITE_ALPHABLEND);

    if (ViewClass) {
        //DrawTextWithoutOffset(NameWeaponPossition, (char*)"Weapon");
        //DrawTextWithoutOffset(NameStatsStrPossition, (char*)"STR");
        //DrawTextWithoutOffset(NameStatsIntPossition, (char*)"INT");
        //DrawTextWithoutOffset(NameStatsDexPossition, (char*)"DEX");

        DrawTextWithoutOffset(NameWeaponPossition, (char*)"Оружие");
        DrawTextWithoutOffset(NameStatsStrPossition, (char*)"Сила");
        DrawTextWithoutOffset(NameStatsIntPossition, (char*)"Интелект");
        DrawTextWithoutOffset(NameStatsDexPossition, (char*)"Ловкость");

        if (CurrentClass == WA) {
            DrawClassTexture(CurrentCharacterNameWAInDds, CurrentCharacterNamePosition);
            //DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Warrior");
            //DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"The best melee fighter who possesses powerful fencing skills");
            //DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Successors of 'the earliest Rules of the Force'. Training sticly focuses on physical strength, therefore preferring strong and reliable armours such as \n'Armor Suit Set' from the Midreth Continent.");
            //DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Great Sword or Daikatana");

            DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Воин");
            DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"Лучший боец ближнего боя, обладающий мощными навыками.");
            DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Воины, их тяжелые тренировки сосредоточены на физической силе, поэтому они предпочитают крепкие и надежные доспехи, такие как \n«Бронированные доспехи» с континента Мидрет.");
            DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Двуручный меч или Дайкатана");

            DrawTextWithoutOffsetGrey(NameStatsStrTextPossition, (char*)"24");
            DrawTextWithoutOffsetGrey(NameStatsIntTextPossition, (char*)"3");
            DrawTextWithoutOffsetGrey(NameStatsDexTextPossition, (char*)"8");
        }

        if (CurrentClass == BL) {
            DrawClassTexture(CurrentCharacterNameBLInDds, CurrentCharacterNamePosition);
            //DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Blader");
            //DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"The deadly blade dancer, the fastest dual sword user");
            //DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Successors of 'the intermediate Rules of the Force' originated from ancient easten martial art focusing on speed. They either use one single-edged sword or two double-edger swords. Skills require them to move quickly and nimbly, thus they prefer light 'Martial Suit Set' from the Huan Continent as South.");
            //DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Blade or Katana");

            DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Ассасин");
            DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"Смертоносный танцор с клинками, самый быстрый владелец парных мечей");
            DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Преемники древнего восточного боевого искусства, ориентированного на скорость. Они используют два обоюдоострых меча. Навыки требуют, чтобы они двигались быстро и проворно, поэтому они предпочитают \n«Легкие доспехи» с юга континента Хуан.");
            DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Мечи или Катаны");

            DrawTextWithoutOffsetGrey(NameStatsStrTextPossition, (char*)"16");
            DrawTextWithoutOffsetGrey(NameStatsIntTextPossition, (char*)"3");
            DrawTextWithoutOffsetGrey(NameStatsDexTextPossition, (char*)"16");
        }

        if (CurrentClass == GL) {
            DrawClassTexture(CurrentCharacterNameGLInDds, CurrentCharacterNamePosition);
            //DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Gladiator");
            //DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"Strength gained by uprising rage");
            //DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Gladiator can control the anger towards their opponent, converting into a pure forn of rage to create a mighty force of strength. Specialized in hard trained physical abilities, they prefer 'Armor Suit Set' to pull their strenghts to the limits");
            //DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Chakrams");

            DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Гладиатор");
            DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"Сила, полученная восстанием ярости");
            DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Гладиатор может контролировать гнев по отношению к своему противнику превращая его в чистую форму ярости. Специализируясь на упорно тренируемых физических способностях, они предпочитают \n«Бронированные доспехи», чтобы довести свои силы до предела.");
            DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Чакрамы");

            DrawTextWithoutOffsetGrey(NameStatsStrTextPossition, (char*)"27");
            DrawTextWithoutOffsetGrey(NameStatsIntTextPossition, (char*)"3");
            DrawTextWithoutOffsetGrey(NameStatsDexTextPossition, (char*)"5");
        }

        if (CurrentClass == FB) {
            DrawClassTexture(CurrentCharacterNameFBInDds, CurrentCharacterNamePosition);
            //DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Force Blader");
            //DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"Swordsman whose blade flares with the force");
            //DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Force Bladers are the final successors of the Rules of the Force. They use orb and one-handed sword to cast aggressive forms of the force and to physically attack at the same time. They value physical balance and coordination, \nthus prefer 'Battle Suit Set'.");
            //DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Katana and Orb");

            DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Ведьмак");
            DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"Мечник, чей клинок вспыхивает с силой");
            DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Ведьмаки используют сферу и одноручный меч, чтобы использовать агрессивные заклинания и одновременно физические атаки. \nОни ценят физическое равновесие и координацию, \nпоэтому предпочитают «Боевые доспехи».");
            DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Катана и Сфера");

            DrawTextWithoutOffsetGrey(NameStatsStrTextPossition, (char*)"12");
            DrawTextWithoutOffsetGrey(NameStatsIntTextPossition, (char*)"12");
            DrawTextWithoutOffsetGrey(NameStatsDexTextPossition, (char*)"11");
        }

        if (CurrentClass == FS) {
            DrawClassTexture(CurrentCharacterNameFSInDds, CurrentCharacterNamePosition);
            //DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Force Shielder");
            //DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"The faithful warrior thet uses the force to shield others");
            //DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Force Shielders hold Crystals to manifest an 'Astral Shield' and use \none-handed sword to attack. Since they can equip the 'Astral Shield', they possess the highest defence among all the battle styles. Specialized in defending, Force Shields preger strong and reliable 'Armor Suit Set'");
            //DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Blade and Crystal");

            DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Защитник");
            DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"Верный воин, который использует силу, чтобы защитить других");
            DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Защитники используют кристаллы, чтобы создать «астральный щит», \nи используют одноручный меч для атаки. Поскольку они используют \n«Астральный щит», они обладают самой высокой защитой среди всех классов.");
            DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Меч и Кристал");

            DrawTextWithoutOffsetGrey(NameStatsStrTextPossition, (char*)"15");
            DrawTextWithoutOffsetGrey(NameStatsIntTextPossition, (char*)"11");
            DrawTextWithoutOffsetGrey(NameStatsDexTextPossition, (char*)"9");
        }

        if (CurrentClass == FA) {
            DrawClassTexture(CurrentCharacterNameFAInDds, CurrentCharacterNamePosition);
            //DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Force Archer");
            //DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"The sniper that fires deadly force shots that cut through the wind");
            //DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Originated from Wizard faction, Wizard developed various ways of manifesting the force to attack from a distance. They mainly use Crystals as their weapons, however they also use 'Astral Bows' as the carriers of the force to control the force more efficiently. They prefer 'Battle Suit Set' from the Pastur Continent.");
            //DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Crystals");

            DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Стрелок");
            DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"Снайпер, стреляющий смертоносными выстрелами, прорезающими ветер");
            DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Стрелок создан во фракции Волшебников, Волшебники разработали различные способы проявления силы для атаки на расстоянии. \nВ основном они используют кристаллы в качестве оружия. \nОни предпочитают «Боевые доспехи» с континента Пастур.");
            DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Кристалы");

            DrawTextWithoutOffsetGrey(NameStatsStrTextPossition, (char*)"6");
            DrawTextWithoutOffsetGrey(NameStatsIntTextPossition, (char*)"17");
            DrawTextWithoutOffsetGrey(NameStatsDexTextPossition, (char*)"12");
        }

        if (CurrentClass == WI) {
            DrawClassTexture(CurrentCharacterNameWIInDds, CurrentCharacterNamePosition);
            //DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Wizard");
            //DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"Ultimate Destroyer, the Ruler of the Force");
            //DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Third seccessors inherited from the Sage Tower, who manifest the force in its purest form. \nThey use weapon called 'Force Amplifier - Orb', worn on both wrists. \nFor armors, they prefer light 'Martial Suit Set'.");
            //DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Orbs");

            DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Маг");
            DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"Абсолютный Разрушитель, Повелитель Силы");
            DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Маги проявляют силу в ее чистейшей форме. Они используют оружие под названием «Сфера», которое носят на обоих запястьях. Что касается доспехов, они предпочитают «Легкие доспехи».");
            DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Сферы");

            DrawTextWithoutOffsetGrey(NameStatsStrTextPossition, (char*)"3");
            DrawTextWithoutOffsetGrey(NameStatsIntTextPossition, (char*)"26");
            DrawTextWithoutOffsetGrey(NameStatsDexTextPossition, (char*)"6");
        }

        if (CurrentClass == FG) {
            DrawClassTexture(CurrentCharacterNameFGInDds, CurrentCharacterNamePosition);
            //DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Force Gunner");
            //DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"A genius mechanic and Force user");
            //DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Secret agents talented in using force and machines. They can make accurate shots bindfolded by maximizing the power of Astral Weapon and train to improve INT/DEX. Force Gunners prefers 'Battle Suit Set' for more balance.");
            //DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Crystals or Orbs");

            DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Штурмовик");
            DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"Гениальный механик и пользователь силы");
            DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Секретные агенты талантливы в использовании силы и машин. Они могут делать точные выстрелы, максимизируя силу Астрального Оружия. Штурмовик предпочитает «Боевые доспехи» для большего баланса.");
            DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Кристалы или Сферы");

            DrawTextWithoutOffsetGrey(NameStatsStrTextPossition, (char*)"5");
            DrawTextWithoutOffsetGrey(NameStatsIntTextPossition, (char*)"25");
            DrawTextWithoutOffsetGrey(NameStatsDexTextPossition, (char*)"5");
        }

        if (CurrentClass == DM) {
            //DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Dark Mage");
            //DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"Handler of twisted force");
            //DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Dark Mage combinds Soul and Force to create a new twisted force. They can neutralize enemies through power of souls. Souls are handled with orbs on both hands, and freffers light armors.");
            //DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Skull and Sickle");

            DrawHightTextWithoutOffset(NameTitleTextPossition, (char*)"Тёмный маг");
            DrawTextWithoutOffset(NameDescriptionTextPossition, (char*)"Обладатель тёмной силы");
            DrawTextWithoutOffsetGrey(NameFullDescriptionTextPossition, (char*)"Темный маг объединяет Душу и Силу, чтобы создать новую 'Тёмную' силу. Они могут нейтрализовать врагов с помощью силы душ. \nДуши управляются черепом и серпом, \nони предпочитают «Легкие доспехи».");
            DrawTextWithoutOffsetGrey(NameWeaponTextPossition, (char*)"Серп и череп");

            DrawTextWithoutOffsetGrey(NameStatsStrTextPossition, (char*)"4");
            DrawTextWithoutOffsetGrey(NameStatsIntTextPossition, (char*)"26");
            DrawTextWithoutOffsetGrey(NameStatsDexTextPossition, (char*)"5");
        }

        if (ActiveClassTab == TabSW) {
            DrawTexture(imagePosDisactiveTabInDds, TabMAPos);
            DrawTexture(imagePosActiveTabInDds, TabSWPos);

            //DrawTextWithOffset(classWATextPos, (char*)"Warrior");
            //DrawTextWithOffset(classBLTextPos, (char*)"Blader");
            //DrawTextWithOffset(classGLTextPos, (char*)"Gladiator");
            //DrawTextWithOffset(classFBTextPos, (char*)"Force Blader");
            //DrawTextWithOffset(classFSTextPos, (char*)"Force Shielder");
            DrawTextWithOffset(classWATextPos, (char*)"Воин");
            DrawTextWithOffset(classBLTextPos, (char*)"Ассасин");
            DrawTextWithOffset(classGLTextPos, (char*)"Гладиатор");
            DrawTextWithOffset(classFBTextPos, (char*)"Ведьмак");
            DrawTextWithOffset(classFSTextPos, (char*)"Защитник");

            if (CurrentClass == WA) {
                DrawTexture(imagePosWAActiveInDds, classWAPos);
            }
            else {
                DrawTexture(imagePosWAInDds, classWAPos);
            }

            if (CurrentClass == BL) {
                DrawTexture(imagePosBLActiveInDds, classBLPos);
            }
            else {
                DrawTexture(imagePosBLInDds, classBLPos);
            }

            if (CurrentClass == GL) {
                DrawTexture(imagePosGLActiveInDds, classGLPos);
            }
            else {
                DrawTexture(imagePosGLInDds, classGLPos);
            }

            if (CurrentClass == FB) {
                DrawTexture(imagePosFBActiveInDds, classFBPos);
            }
            else {
                DrawTexture(imagePosFBInDds, classFBPos);
            }

            if (CurrentClass == FS) {
                DrawTexture(imagePosFSActiveInDds, classFSPos);
            }
            else {
                DrawTexture(imagePosFSInDds, classFSPos);
            }
        }

        if (ActiveClassTab == TabMA) {
            DrawTexture(imagePosActiveTabInDds, TabMAPos);
            DrawTexture(imagePosDisactiveTabInDds, TabSWPos);

            //DrawTextWithOffset(classFATextPos, (char*)"Force Archer");
            //DrawTextWithOffset(classWITextPos, (char*)"Wizard");
            //DrawTextWithOffset(classFGTextPos, (char*)"Force Gunner");
            //DrawTextWithOffset(classDMTextPos, (char*)"Dark Mage");
            DrawTextWithOffset(classFATextPos, (char*)"Стрелок");
            DrawTextWithOffset(classWITextPos, (char*)"Маг");
            DrawTextWithOffset(classFGTextPos, (char*)"Штурмовик");
            DrawTextWithOffset(classDMTextPos, (char*)"Тёмный маг");

            if (CurrentClass == FA) {
                DrawTexture(imagePosFAActiveInDds, classFAPos);
            }
            else {
                DrawTexture(imagePosFAInDds, classFAPos);
            }

            if (CurrentClass == WI) {
                DrawTexture(imagePosWIActiveInDds, classWIPos);
            }
            else {
                DrawTexture(imagePosWIInDds, classWIPos);
            }

            if (CurrentClass == FG) {
                DrawTexture(imagePosFGActiveInDds, classFGPos);
            }
            else {
                DrawTexture(imagePosFGInDds, classFGPos);
            }

            if (CurrentClass == DM) {
                DrawDMTexture(imagePosDMActiveInDds, classDMPos);
            }
            else {
                DrawDMTexture(imagePosDMInDds, classDMPos);
            }
        }

        DrawNameTexture(imageNamePosInDds, NamePosition);
        //DrawTextWithOffset(NameTextPossition, (char*)"Character Name");
        DrawTextWithOffset(NameTextPossition, (char*)"Имя персонажа");

        //DrawTextWithOffset(TabSWTextPos, (char*)"Physical");
        //DrawTextWithOffset(TabMATextPos, (char*)"Magical");
        DrawTextWithOffset(TabSWTextPos, (char*)"Физические");
        DrawTextWithOffset(TabMATextPos, (char*)"Магические");

        DrawMouse(_this);
    }

    if (*(DWORD*)(*(DWORD*)(PGlobal)+LOGIN_ON_AND_OFF123) == 1 && *(DWORD*)isNpcOpen == 0)
    {
        POINT pCVar;
        GetCursorPos(&pCVar);
        ScreenToClient(GetActiveWindow(), &pCVar);

        if (GetKeyState(VK_LBUTTON) & 0x80)
        {
            if (pCVar.x >= MenuX && pCVar.y >= MenuY && pCVar.x <= MenuX + imageSize.right && pCVar.y <= MenuY + imageSize.bottom)
            {
                MenuX = pCVar.x - xDifference;
                MenuY = pCVar.y - yDifference;

                isMenuMoving = TRUE;
            }

            if (isMenuMoving)
            {
                if (MenuX < 0)
                    MenuX = 0;
                if (MenuY < 0)
                    MenuY = 0;
                if (MenuX + imageSize.right > Viewport.Width)
                    MenuX = Viewport.Width - imageSize.right;
                if (MenuY + imageSize.bottom > Viewport.Height)
                    MenuY = Viewport.Height - imageSize.bottom;
            }
        }
        else
        {
            xDifference = (pCVar.x - MenuX);
            yDifference = (pCVar.y - MenuY);
            isMenuMoving = FALSE;
        }

        DWORD h1 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_HONRA);

        DWORD a1 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_FOR);
        DWORD a2 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_INT);
        DWORD a3 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_AMP_E);
        DWORD a4 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_AMP_M);
        DWORD a5 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_ATTACK);
        DWORD a6 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_ATTACK_M);
        DWORD a7 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_ATTACK_ALL);
        DWORD a8 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_AMP_ALL);
        DWORD a9 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_DANO_ADC);
        DWORD a10 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_ABC_HP_MP);
        DWORD a11 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_LMT_ABS_HP);
        DWORD a12 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_LMT_ABS_MP);
        DWORD a13 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_DANO_MIN);
        DWORD a14 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_DC);
        DWORD a15 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_TAXA);
        DWORD a16 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_MAX_TAXA);
        DWORD a17 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_PREC);



        DWORD b1 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_DES);
        DWORD b2 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_HP);
        DWORD b3 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_MP);
        DWORD b4 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_RESS_TAXA);
        DWORD b5 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_RESS_DC);
        DWORD b6 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_RESS_AMP);
        DWORD b7 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_ESQUIVA);
        DWORD b8 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_AUT_CURA_HP);
        DWORD b9 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_AUT_CURA_MP);
        DWORD b10 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_DEF);
        DWORD b11 = *(DWORD*)(*(DWORD*)(PGlobal)+Atribut_EVA);

        DWORD PA = (0.00025 * h1 / 200) + b1 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 * 5 + a9 + a10 + a11 + a12 + a13 + a14 * 5 + a15 + a17;
        DWORD PD = (0.0003 * h1 / 200) + b1 + b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + b10 + b11;

        char PATX[MAXCHAR], PDTX[MAXCHAR];
        wsprintfA(PATX, "%d", PA);
        wsprintfA(PDTX, "%d", PD);

        RECT TextPA = { MenuX + 15, MenuY + 4, MenuX + 70, MenuY + 16 };
        RECT TextPD = { MenuX + 70, MenuY + 4, MenuX + 120, MenuY + 16 };
        D3DXVECTOR3 pos = { float(MenuX), float(MenuY), 0.0f };
        DrawTextureWithoutOffset(imageSize, pos);
        DrawTextWithoutOffsetColor(TextPA, PATX, D3DCOLOR_ARGB(255, 255, 0, 0));
        DrawTextWithoutOffsetColor(TextPD, PDTX, D3DCOLOR_ARGB(255, 0, 0, 255));

        if (GetClassId(*(DWORD*)(*(DWORD*)(PGlobal)+CURRENT_CLASS)) == 7) {
            DrawTextureRagePoints({ float(260), float(8), 0.0f });
        }

        if (((*(DWORD*)MAP_IDS) == 53)) {
            pos = { float(5), float(300), 0.0f };
            RECT timeTextSize = { 70, 375, 250, 435 };
            RECT dmgTextSize = { 70, 430, 250,490 };
            RECT imageSizeBaldus = { 0, 0, 260, 170 };

            DrawTextureBaldus(imageSizeBaldus, pos);
            DWORD damage = *(DWORD*)(0x010CDC54);
            char BuffDMG[MAXCHAR];
            wsprintfA(BuffDMG, "%d", damage);

            DWORD time = *(DWORD*)(0x0FAC727);
            char BuffTime[MAXCHAR];
            int intTime = (int)time / 256000;
            wsprintfA(BuffTime, "%d", intTime);

            DrawTextWithoutOffset(dmgTextSize, (char*)BuffDMG);
            DrawTextWithoutOffset(timeTextSize, (char*)BuffTime);
        }
    }

    DrawTextureXPoints({ float(79), float(67), 0.0f });

    SptMenu->End();
}

void Reset(Direct3DDevice9Wrapper* _this)
{
    ReleaseElement((LPUNKNOWN&)SptMenu);
    ReleaseElement((LPUNKNOWN&)BaldusTexture);
    ReleaseElement((LPUNKNOWN&)PAPDTexture);
    ReleaseElement((LPUNKNOWN&)ClassTexture);
    ReleaseElement((LPUNKNOWN&)DMClassTexture);
    ReleaseElement((LPUNKNOWN&)EnterNameTexture);
    ReleaseElement((LPUNKNOWN&)miniFont);
    ReleaseElement((LPUNKNOWN&)font);
    ReleaseElement((LPUNKNOWN&)hightFont);
}

void hkCreateCharacter()
{
    ViewClass = true;
    CurrentClass = WA;
    ActiveClassTab = TabSW;

    oCreateCharacter();
}

void hkExitCreateCharacter()
{
    ViewClass = false;

    oExitCreateCharacter();
}

void hkExitAfterCreateCharacter()
{
    ViewClass = false;

    oExitAfterCreateCharacter();
}

DWORD RetIncrementXPoints;

void __declspec(naked) IncrementXPoints() 
{
    //VMProtectBegin("IncXPoints");
    __asm {
        PUSH    EAX
        PUSH    EDX

        MOV     EAX, dword ptr ds : [0xB93530]
        ADD     EAX, 0x41B0
        MOV     EAX, dword ptr ds : [EAX]

        MOV     EDX, EAX
        SUB     EDX, 0x20
        CMP     EDX, 0
        JE      RET_INC
        
        SUB     EDX, 0x20
        CMP     EDX, 0
        JE      RET_INC

        MOV     EDX, EAX
        SUB     EDX, 0x22
        CMP     EDX, 0
        JE      RET_INC

        SUB     EDX, 0x20
        CMP     EDX, 0
        JE      RET_INC

        MOV     EDX, EAX
        SUB     EDX, 0x28
        CMP     EDX, 0
        JE      RET_INC

        SUB     EDX, 0x20
        CMP     EDX, 0
        JE      RET_INC

        MOV     EDX, EAX
        SUB     EDX, 0x2A
        CMP     EDX, 0
        JE      RET_INC

        SUB     EDX, 0x20
        CMP     EDX, 0
        JE      RET_INC

        CMP     XPoints, 177
        JE      ADD_RAGE
        ADD     XPoints, 1
        JMP     ADD_RAGE

        ADD_RAGE : 
            CMP     RagePoints, 11
            JE      RET_INC
            ADD     RagePoints, 1
            JMP     RET_INC

        RET_INC : 
            POP     EDX
            POP     EAX
            JMP     RetIncrementXPoints
    }
    //VMProtectEnd();
}

DWORD RetDefLoad = 0x00549B32;
void __declspec(naked) NullableXPoints() {
    __asm {
        mov     XPoints, 0
        mov     RagePoints, 0
        JMP		RetDefLoad
    }
}

DWORD Call_00429FE1 = 0x00429FE1, Ret_00576BF1 = 0x00576BF1, loc_576DFC = 0x00576DFC;

void __stdcall PrintXSkillBlock()
{
    typedef void(__thiscall* t_Draw) (void*, char*, int, int);
    t_Draw CabalDrawText = (t_Draw)0x0043866D;

    DWORD dwPrintClass = 0x00BA3CA0;
    PVOID pPrintClass = (PVOID)dwPrintClass;
    char DUMP[MAXCHAR] = "Вам не хватает Энергии Титана!";

    CabalDrawText(pPrintClass, DUMP, 0x1C, 7);
}

void __stdcall PrintRSkillBlock()
{
    typedef void(__thiscall* t_Draw) (void*, char*, int, int);
    t_Draw CabalDrawText = (t_Draw)0x0043866D;

    DWORD dwPrintClass = 0x00BA3CA0;
    PVOID pPrintClass = (PVOID)dwPrintClass;
    char DUMP[MAXCHAR] = "Вам не хватает Ярости!";

    CabalDrawText(pPrintClass, DUMP, 0x1C, 7);
}

void __declspec(naked) SkillBlock() {
    //VMProtectBegin("SkillBlock");
    __asm {
        CMP		dword ptr[esp + 0xC], 397//Щит тамплиера
        JE		CHECK_RAGE_POINTS

        CMP		dword ptr[esp + 0xC], 181//Молитва тамплиера
        JE		CHECK_RAGE_POINTS

        CMP		dword ptr[esp + 0xC], 47
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 48
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 49
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 50
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 67
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 92
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 142
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 143
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 195
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 201
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 278
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 279
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 280
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 295
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 297
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 307
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 308
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 309
        JE		CHECK_POINTS

        CMP		dword ptr[esp + 0xC], 339
        JE		BLOCK_SKILL

        JMP     BACK_TO_CODE

        CHECK_POINTS :
            CMP XPoints, 59
            JGE ACCEPT_POINTS
            CALL PrintXSkillBlock
            JMP BLOCK_SKILL

        CHECK_RAGE_POINTS :
            CMP RagePoints, 6
            JGE ACCEPT_RAGE
            CALL PrintRSkillBlock
            JMP BLOCK_SKILL

        ACCEPT_RAGE :
            SUB     RagePoints, 6
            JMP     BACK_TO_CODE

        ACCEPT_POINTS :
            SUB     XPoints, 59
            JMP     BACK_TO_CODE

        BACK_TO_CODE :
            CALL    Call_00429FE1
            JMP     Ret_00576BF1

        BLOCK_SKILL :
            JMP loc_576DFC
    }
    //VMProtectEnd();
}

DWORD sub_42C647 = 0x42C647;
DWORD sub_40738D = 0x40738D;
DWORD BLOCK_ALL_SKILL = 0x576E75;

void __declspec(naked) SkillBlockAll() {
    //VMProtectBegin("SkillBlockAll");
    __asm {
        CMP		dword ptr[esp + 0xC], 0x00000144//324
        JE		ACTIVATE_BUFF

        JMP     RET_CHECK

        ACTIVATE_BUFF :
            CMP     RagePoints, 10
            JB      RET_CHECK
            push    0x20
            push    0x00000153
            push    0x18
            mov     ecx, 0x10CD198
            call    sub_42C647
            JMP     RET_CHECK

         RET_CHECK :
            call    sub_40738D
            JMP		BLOCK_ALL_SKILL
    }
    //VMProtectEnd();
}

void SendResult()
{
    //VMProtectBegin("Send result");
    typedef void(__thiscall* t_Draw) (void*, char*, int, int);
    t_Draw CabalDrawText = (t_Draw)0x0043866D;

    DWORD dwPrintClass = 0x00BA3CA0;
    PVOID pPrintClass = (PVOID)dwPrintClass;

    char Damage[MAXCHAR];
    wsprintfA(Damage, "%d", *(DWORD*)(0x010CDC54));

    char CharacterIdxString[MAXCHAR];
    wsprintfA(CharacterIdxString, "%d", *(DWORD*)(0x010812A0));

    auto r = cpr::Post(cpr::Url{ "https://cabal-online-titan.online/api/baldus/save-result" },
            cpr::Parameters{{"CharacterIdx", CharacterIdxString}, {"Points", Damage }}
        );

    wsprintfA(Damage, "Ваш урон по балдусу %d", *(DWORD*)(0x010CDC54));

    CabalDrawText(pPrintClass, Damage, 0x1C, 7);
    //VMProtectEnd();
}

int __fastcall CheckExitDungeon(DWORD* _this, void* n)
{
    typedef int(__thiscall* Tsub_672890)(DWORD*);
    Tsub_672890 Osub_672890 = (Tsub_672890)0x00672890;

    int OutMap = *(DWORD*)0xBA0760;
    int inMap = *(DWORD*)(*(DWORD*)(0xB93530) + 0x72E4);

    if (OutMap == 53 && inMap == 1) {
        SendResult();
    }

    if (inMap == 53) {
        (*(DWORD*)(0x010CDC54)) = 0;
    }

    return Osub_672890(_this);
}

void __declspec(naked) IconOnC() {
    __asm {
        lea esp, [esp + 4]
        pushad
        movzx ax, ch
        pushfd
        mov eax, [0x00B93530]
        push eax
    }
}

struct BM3SkillsStruct
{
    int SlotID;
    int SkillID;
    int Timeout;
};

std::map <int, std::map <int, BM3SkillsStruct>> BMSkills = {
    { 7, {
            { 1 , { 105, 325, 1250 } },
            { 2 , { 106, 326, 1250 } },
            { 3 , { 105, 325, 1250 } },
            { 4 , { 105, 325, 1250 } },

            { 5 , { 106, 326, 1250 } },
            { 6 , { 105, 325, 1250 } },
            { 7 , { 105, 325, 1250 } },
            { 8 , { 106, 326, 1250 } },

            { 17 , { 107, 327, 2050 } },
        }
    },
};

int AutoBM3Stage = 1;

DWORD WINAPI ActivateBM3Skills(LPVOID lpParam)
{
    typedef DWORD* (__thiscall* Tsub_577D5A)(DWORD*);
    Tsub_577D5A Osub_577D5A = (Tsub_577D5A)0x00577D5A;

    Sleep(5000);

    while (true) {
        int CurrentBMState = *(DWORD*)(*(DWORD*)(PGlobal) + CURRENT_BM_STATE);

        if (CurrentBMState == 0x40) {
            int CurrentClassID = GetClassId(*(DWORD*)(*(DWORD*)(PGlobal) + CURRENT_CLASS));

            auto SkillList = BMSkills.find(CurrentClassID);

            for (auto Skill = SkillList->second.begin(); Skill != SkillList->second.end(); Skill++)
            {
                if (Skill->first != 17 &&
                        (
                            (AutoBM3Stage == 1 && Skill->first > 4) ||
                            (AutoBM3Stage == 2 && Skill->first > 8) ||
                            (AutoBM3Stage == 3 && Skill->first > 12)
                        )
                    ) continue;

                *(DWORD*)(*(DWORD*)(PGlobal) + 0x4830) = Skill->second.SlotID;
                *(DWORD*)(*(DWORD*)(PGlobal) + 0x3148) = Skill->second.SkillID;
                Osub_577D5A((DWORD*)(*(DWORD*)(PGlobal)));
                Sleep(Skill->second.Timeout);
            }
        }
    }

    return 0;
}

DWORD WINAPI SetHook(LPVOID lpParam)
{
    //VMProtectBegin("Init");
    Hook::GetInstance()->SetHook(0x0041E001, (DWORD)CheckExitDungeon, 0xE9);
    Hook::GetInstance()->SetHook(0x00576BEC, (DWORD)SkillBlock, 0xE9);
    
    Hook::GetInstance()->SetHook(0x00576E70, (DWORD)SkillBlockAll, 0xE9);

    AddOnNotifyCreateDevice(NotifyCreateDevice);
    AddOnPresent(Present);
    AddOnReset(Reset);

    SetupD3DHook();

    oCreateCharacter = (tCreateCharacter)((PVOID*)(0x66CF11));
    oExitCreateCharacter = (tExitCreateCharacter)((PVOID*)(0x736667));
    oExitAfterCreateCharacter = (tExitAfterCreateCharacter)((PVOID*)(0x763A5B));
    oAnimationCreateCharacter = (tAnimationCreateCharacter)((PVOID*)(0x735CE2));

    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)oCreateCharacter, hkCreateCharacter);
    DetourAttach(&(PVOID&)oExitCreateCharacter, hkExitCreateCharacter);
    DetourAttach(&(PVOID&)oExitAfterCreateCharacter, hkExitAfterCreateCharacter);
    if (DetourTransactionCommit() != NO_ERROR) {
        MessageBox(NULL, L"ERROR", L"Hook", 0);
    }

    Sleep(10000);
    RetIncrementXPoints = (DWORD)(*(DWORD*)0x00414D59 + 0x00414D5D);
    Hook::GetInstance()->SetHook(0x00414D58, (DWORD)IncrementXPoints, 0xE9);
    Hook::GetInstance()->SetHook(0x0043240C, (DWORD)NullableXPoints, 0xE9);

    while (true) {
        if (RagePoints > 0) {
            RagePoints--;
        }

        Sleep(10000);
    }

    //VMProtectEnd();

    return 0;
}

void Patch(DWORD dst, BYTE* src, const intptr_t size)
{
    DWORD oldprotect;

    VirtualProtect((PVOID)dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
    std::memcpy((PVOID)dst, src, size);
    VirtualProtect((PVOID)dst, size, oldprotect, &oldprotect);
}

void DrawTextureBaldus(RECT imagePosInDds, D3DXVECTOR3 Position)
{
    SptMenu->Draw(BaldusTexture, &imagePosInDds, NULL, &Position, 0xFFFFFFFF);
}

void DrawTextureClassChange(RECT imagePosInDds, D3DXVECTOR3 Position)
{
    Position.x += Viewport.Width - 345;
    Position.y += Viewport.Height / 2 - 232;

    SptMenu->Draw(ClassTexture, &imagePosInDds, NULL, &Position, 0xFFFFFFFF);
}

void DrawTextWithoutOffset(RECT TextPos, char text[])
{
    font->DrawTextA(SptMenu, text, -1, &TextPos, DT_LEFT, D3DCOLOR_ARGB(255, 112, 178, 238));
}

void DrawClassTexture(RECT imagePosInDds, D3DXVECTOR3 Position)
{
    SptMenu->Draw(ClassTexture, &imagePosInDds, NULL, &Position, 0xFFFFFFFF);
}

void DrawDMTexture(RECT imagePosInDds, D3DXVECTOR3 Position)
{
    Position.x += Viewport.Width - 345;
    Position.y += Viewport.Height / 2 - 232;

    SptMenu->Draw(DMClassTexture, &imagePosInDds, NULL, &Position, 0xFFFFFFFF);
}

void DrawHightTextWithoutOffset(RECT TextPos, char text[])
{
    hightFont->DrawTextA(SptMenu, text, -1, &TextPos, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void DrawNameTexture(RECT imagePosInDds, D3DXVECTOR3 Position)
{
    Position.x += Viewport.Width - 345;
    Position.y += Viewport.Height / 2 - 232;

    SptMenu->Draw(EnterNameTexture, &imagePosInDds, NULL, &Position, 0xFFFFFFFF);
}

void DrawTexture(RECT imagePosInDds, D3DXVECTOR3 Position)
{
    Position.x += Viewport.Width - 345;
    Position.y += Viewport.Height / 2 - 232;

    SptMenu->Draw(ClassTexture, &imagePosInDds, NULL, &Position, 0xFFFFFFFF);
}

void DrawTextWithOffset(RECT TextPos, char text[])
{
    TextPos.left += Viewport.Width - 345;
    TextPos.top += Viewport.Height / 2 - 232;
    TextPos.right += Viewport.Width - 345;
    TextPos.bottom += Viewport.Height / 2 - 232;

    font->DrawTextA(SptMenu, text, -1, &TextPos, DT_VCENTER | DT_CENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 200, 200, 200));
}

void DrawTextWithoutOffsetGrey(RECT TextPos, char text[])
{
    font->DrawTextA(SptMenu, text, -1, &TextPos, DT_LEFT | DT_WORDBREAK, D3DCOLOR_ARGB(255, 175, 175, 175));
}

void DrawMouse(LPDIRECT3DDEVICE9 pDevice)
{
    GetCursorPos(&CursorPos);
    ScreenToClient(GetActiveWindow(), &CursorPos);

    if (GetKeyState(VK_LBUTTON) & 0x80)
    {
        if (ActiveClassTab == TabSW) {
            bool isWA = CheckClick(classWAPos, imagePosWAInDds);

            if (isWA && CurrentClass != WA) {
                Activate(WA);

                return;
            }

            bool isBL = CheckClick(classBLPos, imagePosBLInDds);

            if (isBL && CurrentClass != BL) {
                Activate(BL);

                return;
            }

            bool isGL = CheckClick(classGLPos, imagePosGLInDds);

            if (isGL && CurrentClass != GL) {
                Activate(GL);

                return;
            }

            bool isFB = CheckClick(classFBPos, imagePosFBInDds);

            if (isFB && CurrentClass != FB) {
                Activate(FB);

                return;
            }

            bool isFS = CheckClick(classFSPos, imagePosFSInDds);

            if (isFS && CurrentClass != FS) {
                Activate(FS);

                return;
            }

            bool isTabMA = CheckClick(TabMAPos, imagePosActiveTabInDds);

            if (isTabMA) {
                ActiveClassTab = TabMA;
            }
        }

        if (ActiveClassTab == TabMA) {
            bool isFA = CheckClick(classFAPos, imagePosFAInDds);

            if (isFA && CurrentClass != FA) {
                Activate(FA);

                return;
            }

            bool isWI = CheckClick(classWIPos, imagePosWIInDds);

            if (isWI && CurrentClass != WI) {
                Activate(WI);

                return;
            }

            bool isFG = CheckClick(classFGPos, imagePosFGInDds);

            if (isFG && CurrentClass != FG) {
                Activate(FG);

                return;
            }

            bool isDM = CheckClick(classDMPos, imagePosDMInDds);

            if (isDM && CurrentClass != DM) {
                Activate(DM);

                return;
            }

            bool isTabSW = CheckClick(TabSWPos, imagePosActiveTabInDds);

            if (isTabSW) {
                ActiveClassTab = TabSW;
            }
        }
    }
}

bool CheckClick(D3DXVECTOR3 Pos, RECT ImagePos)
{
    Pos.x += Viewport.Width - 345;
    Pos.y += Viewport.Height / 2 - 232;

    if (
        (CursorPos.x > (Pos.x))
        && (CursorPos.x < (Pos.x + ImagePos.right - ImagePos.left))
        && (CursorPos.y > Pos.y)
        && (CursorPos.y < (Pos.y + ImagePos.bottom - ImagePos.top))
        ) {
        return true;
    }

    return false;
}

void DrawAnimationCreateCharacter() {

    switch (CurrentClass)
    {
    case WA:
        oAnimationCreateCharacter(&WAANIE);
        break;
    case BL:
        oAnimationCreateCharacter(&BLANIE);
        break;
    case GL:
        oAnimationCreateCharacter(&GLANIE);
        break;
    case FB:
        oAnimationCreateCharacter(&FBANIE);
        break;
    case FS:
        oAnimationCreateCharacter(&FSANIE);
        break;
    case FA:
        oAnimationCreateCharacter(&FAANIE);
        break;
    case WI:
        oAnimationCreateCharacter(&WIANIE);
        break;
    case FG:
        oAnimationCreateCharacter(&FGANIE);
        break;
    case DM:
        oAnimationCreateCharacter(&DMANIE);
        break;
    default:
        oAnimationCreateCharacter(&WAANIE);
    }
}

void Activate(BYTE classByte)
{
    BYTE dmbytes[4] = { 0x00, 0x00, 0x00, 0x00 };
    DWORD handle = (DWORD)GetModuleHandleA("cabalmain.exe");

    DWORD* BaseAddress = (DWORD*)(handle + 0xC7BF5C);
    DWORD offsetAddress = (*BaseAddress + 0x41A8);

    std::memcpy(dmbytes, (PVOID)offsetAddress, 4);

    bool gender = false;

    if (dmbytes[3] == 0x04 || dmbytes[3] == 0x24 || dmbytes[3] == 0x34 || dmbytes[3] == 0x44)
    {
        gender = true;
    }

    switch (classByte)
    {
    case WA:
        dmbytes[3] = gender ? 0x04 : 0x00;          //A1 00 C0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        memcpy((PVOID)0x107C868, WA_ITEMS, 20);
        break;
    case BL:
        dmbytes[3] = gender ? 0x04 : 0x00;          //A2 00 C0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        break;
    case GL:
        dmbytes[3] = gender ? 0x24 : 0x20;          //A1 00 C0 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        memcpy((PVOID)0x107C868, GL_ITEMS, 20);
        break;
    case FB:
        dmbytes[3] = gender ? 0x04 : 0x00;
        break;
    case FS:
        dmbytes[3] = gender ? 0x04 : 0x00;
        break;
    case FA:
        dmbytes[3] = gender ? 0x04 : 0x00;          //A4 00 C0 00 00 00 00 00 02 00 00 00 00 00 00 00 00 00 00 00
        break;
    case WI:
        dmbytes[3] = gender ? 0x04 : 0x00;
        memcpy((PVOID)0x0107C910, WI_ITEMS, 20);    //A3 00 C0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        break;
    case FG:
        dmbytes[3] = gender ? 0x34 : 0x30;          //A4 00 C0 30 00 00 00 00 02 00 00 00 00 00 00 00 00 00 00 00
        break;
    case DM:
        dmbytes[3] = gender ? 0x44 : 0x40;          //A3 00 C0 44 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
        memcpy((PVOID)0x0107C910, DM_ITEMS, 20);
        break;
    default:
        dmbytes[3] = gender ? 0x04 : 0x00;
    }

    Patch(offsetAddress, dmbytes, 4);

    CurrentClass = classByte;

    DrawAnimationCreateCharacter();
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, SetHook, 0, 0, 0);
        CreateThread(0, 0, ColorNames, 0, 0, 0);
        //CreateThread(0, 0, ActivateBM3Skills, 0, 0, 0);
    }

    return TRUE;
}