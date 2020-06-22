#pragma once

#include <vector>
#include <windows.h>
#include <TlHelp32.h>


DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAdress(DWORD procId, const wchar_t* modName);

uintptr_t FindMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);