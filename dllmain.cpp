#include "pch.h"

BYTE ASM[] = { 0xBA, 0x00, 0x02, 0x00, 0x00, 0x41, 0xB8, 0x00, 0x02, 0x00, 0x00 }; // mov edx, 200h, mov r8d, 200h
BYTE TARGET = 0x28; // 10240 x 10240

void DoPatch() {
    MODULEINFO moduleInfo;

    if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &moduleInfo, sizeof(moduleInfo))) {
        return;
    }

    BYTE* baseAddress = (BYTE*)moduleInfo.lpBaseOfDll;
    DWORD moduleSize = moduleInfo.SizeOfImage;

    for (DWORD i = 0; i < moduleSize - sizeof(ASM); i++) {
        BOOL found = TRUE;

        for (int j = 0; j < sizeof(ASM); j++) {
            if (baseAddress[i + j] != ASM[j]) {
                found = FALSE;
                break;
            }
        }

        if (found) {
            LPVOID addr = (LPVOID)(baseAddress + i); // i = 0x5948E2 i guess
            DWORD oldProtect = 0;

            if (VirtualProtect(addr, sizeof(ASM), PAGE_EXECUTE_READWRITE, &oldProtect))
            {
                *(BYTE*)((DWORD64)addr + 2) = TARGET;
                *(BYTE*)((DWORD64)addr + 8) = TARGET;

                VirtualProtect(addr, sizeof(ASM), oldProtect, &oldProtect);
            }

            return;
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DoPatch();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}
