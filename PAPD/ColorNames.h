#pragma once
#include "Windows.h"
#include <vector>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "Hook.h"
#include "VMProtectSDK.h"
#pragma comment(lib, "VMProtectSDK32.lib")
DWORD WINAPI ColorNames(LPVOID lpParam);

void DefaultLoadCharactersNameColor();
void SetCharacterColor();
void DefaultSetCharacterColor();
void LoadCharactersNameColor();