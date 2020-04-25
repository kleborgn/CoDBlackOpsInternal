#pragma once
#include "pch.h"
#include <windows.h>
#include <vector>
//https://guidedhacking.com/threads/simple-x86-c-trampoline-hook.14188/

namespace mem
{
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void Nop(BYTE* dst, unsigned int size);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);

	bool Detour32(BYTE* src, BYTE* dst, const uintptr_t len);
	BYTE* TrampHook32(BYTE* src, BYTE* dst, const uintptr_t len);
	MODULEINFO GetModuleInfo(LPCWSTR szModule);
	DWORD FindPattern(LPCWSTR module, char* pattern, char* mask);
}