#include "ColorNames.h"
// Add new column in Server01.dbo.cabal_character_table 
// SQL  -  ALTER TABLE Server01.dbo.cabal_character_table ADD NameColor bigint DEFAULT 4294967295 NULL;
// site need return json this format {"characterIdx":color int} 
// example {"368":4294902015,"8":4279312947} 
// 4279312947 = FF112233  (FF - Transparency 11 - Red 22 - Green 33 - Blue )

using json = nlohmann::json;
using namespace std;

struct sCharacterColor
{
	int Id;
	DWORD CharacterColor;
};

std::vector<sCharacterColor> CharacterNameColors = {};

int CharIdInGame = 0;
DWORD CurrentCharacterColor = 0xFFFFFFFF;
DWORD RetDefColorLoad = 0x00549B37;
DWORD JNL_CharacterColor = 0x004CEF90;
DWORD RetDefaultSetCharacterColor = 0x004CEF49;

DWORD WINAPI ColorNames(LPVOID lpParam)
{
	//VMProtectBegin("ColorN"); 
	Hook::GetInstance()->SetHook(0x004CEF3B, (DWORD)SetCharacterColor, 0xE8);
	Hook::GetInstance()->SetHook(0x004CEF40, (DWORD)DefaultSetCharacterColor, 0xE9);

	Sleep(10000);
	Hook::GetInstance()->SetHook(0x00549B37, (DWORD)LoadCharactersNameColor, 0xE8);
	Hook::GetInstance()->SetHook(0x00549B32, (DWORD)DefaultLoadCharactersNameColor, 0xE9);
	//VMProtectEnd();

	return 0;
}

void __declspec(naked) DefaultLoadCharactersNameColor() 
{
	__asm {
		MOV     EAX, [ESP + 0x4]
		MOV		[ECX + 0x4810], EAX
		JMP		RetDefColorLoad
	}
}

void SetCharacterColor() 
{
	//VMProtectBegin("SetColor");
	__asm {
		PUSH	EAX
		MOV		EAX, ECX
		ADD		EAX, 0x24C
		MOV		EAX, [EAX]
		MOV		dword ptr CharIdInGame, EAX
		POP		EAX
	}

	CurrentCharacterColor = 0xFFFFFFFF;

	for (auto pColor = CharacterNameColors.begin(); pColor != CharacterNameColors.end(); pColor++)
	{
		if (pColor->Id == CharIdInGame)
		{
			CurrentCharacterColor = pColor->CharacterColor;
		}
	}
	//VMProtectEnd();
}

void __declspec(naked) DefaultSetCharacterColor() 
{
	__asm {
		PUSH	EAX
		MOV		EAX, 0x00BF3CFD
		MOV		EDI, dword ptr[CurrentCharacterColor]
		POP		EAX
		JMP		JNL_CharacterColor
	}
}

void LoadCharactersNameColor() 
{
	//VMProtectBegin("LoadColorN");
	auto r = cpr::Get(cpr::Url{ "https://cabal-online-titan.online/api/characters-name-color" });

	CharacterNameColors = {};

	if (json::accept(r.text)) {
		json data = json::parse(r.text);

		for (json::iterator it = data.begin(); it != data.end(); ++it) {
			struct sCharacterColor NewColor = { 0, 0xFFFFFFFF };

			NewColor.Id = stoi(it.key());
			int color = it.value();
			NewColor.CharacterColor = color;

			CharacterNameColors.insert(CharacterNameColors.begin(), NewColor);
		}
	}

	//VMProtectEnd();
}