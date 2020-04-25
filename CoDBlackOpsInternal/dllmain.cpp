// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "mem.h"
#include <iostream>

void displayMenu(HANDLE hConsole, bool bNoClip, bool bGodMode, bool bAmmo) {
	system("cls");
	std::cout << "Injected !" << std::endl;
	if (bNoClip) {
		SetConsoleTextAttribute(hConsole, 10);
	}
	else {
		SetConsoleTextAttribute(hConsole, 14);
	}
	std::cout << "F1 : No Clip (on/off)" << std::endl;
	if (bGodMode) {
		SetConsoleTextAttribute(hConsole, 10);
	}
	else {
		SetConsoleTextAttribute(hConsole, 14);
	}
	std::cout << "F2 : God Mode/Health Loop (on/off)" << std::endl;
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "F3 : Max Money" << std::endl;
	if (bAmmo) {
		SetConsoleTextAttribute(hConsole, 10);
	}
	else {
		SetConsoleTextAttribute(hConsole, 14);
	}
	std::cout << "F4 : Unlimited ammo" << std::endl;
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "INSERT : Uninject" << std::endl;
}

DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole(); //attaches console
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);	//sets cout to be used with our console

	LPCWSTR ProcessName = L"BGamerT5.exe";
	char HealthOpCode[] = "\x89\x85\x84\x01\x00\x00";
	char ammoOpCode[] = "\x89\x50\x04";

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(ProcessName);

	//calling it with NULL also gives the address of the .exe module
	moduleBase = (uintptr_t)GetModuleHandle(NULL);

	bool bNoClip = false, bGodMode = false, bAmmo = false;

	DWORD healthAddress = 0x007DADD0;
	DWORD moneyOffset = 0x180A6C8;
	DWORD ammoAddress = 0x00697A10;
	DWORD noClipAddress = 0x01C0A74C;

	displayMenu(GetStdHandle(STD_OUTPUT_HANDLE), bNoClip, bGodMode, bAmmo);

	while (true)
	{
		if (GetAsyncKeyState(VK_F1) & 1) {
			bNoClip = !bNoClip;

			if (bNoClip)
			{
				displayMenu(GetStdHandle(STD_OUTPUT_HANDLE), bNoClip, bGodMode, bAmmo);
				*(int*)noClipAddress = 1;
			}
			else {
				displayMenu(GetStdHandle(STD_OUTPUT_HANDLE), bNoClip, bGodMode, bAmmo);
				*(int*)noClipAddress = 0;
			}
		}

		if (GetAsyncKeyState(VK_F2) & 1)
		{
			bGodMode = !bGodMode;

			if (bGodMode)
			{
				displayMenu(GetStdHandle(STD_OUTPUT_HANDLE), bNoClip, bGodMode, bAmmo);
				mem::Nop((BYTE*)healthAddress, 6);
			}
			else {
				displayMenu(GetStdHandle(STD_OUTPUT_HANDLE), bNoClip, bGodMode, bAmmo);
				mem::Patch((BYTE*)healthAddress, (BYTE*)HealthOpCode, 6);
			}
		}

		if (GetAsyncKeyState(VK_F3) & 1)
		{
			*(int*)(moduleBase + moneyOffset) = 300000;
		}

		if (GetAsyncKeyState(VK_F4) & 1) {
			bAmmo = !bAmmo;

			if (bAmmo)
			{
				displayMenu(GetStdHandle(STD_OUTPUT_HANDLE), bNoClip, bGodMode, bAmmo);
				mem::Nop((BYTE*)(ammoAddress), 3);
			}
			else {
				displayMenu(GetStdHandle(STD_OUTPUT_HANDLE), bNoClip, bGodMode, bAmmo);
				mem::Patch((BYTE*)(ammoAddress), (BYTE*)ammoOpCode, 3);
			}
		}

		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			std::cout << "Uninjecting..." << std::endl;
			break;
		}


		Sleep(100);
	}

	// Hook

	//

	//cleanup
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

