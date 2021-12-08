// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

struct Vars {
public:
    float customX = 500.f;
    float customZup = 250.f;
    float customZdown = 200.f;
};

Vars* vars;
HMODULE ourHModule;
DWORD dwJMPback = 0;

MODULEINFO GetModuleInfo(const char* szModule)
{
    MODULEINFO modInfo = { 0 };
    HMODULE hModule = ourHModule;

    if (szModule)
    {
        hModule = GetModuleHandleA(szModule);
        if (hModule == 0)
            return modInfo;
    }

    GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
    return modInfo;
}

DWORD FindPattern(const char* module, const char* pattern, const char* mask, int correction)
{
    MODULEINFO mInfo = GetModuleInfo(module);

    DWORD base = (DWORD)mInfo.lpBaseOfDll;
    DWORD size = (DWORD)mInfo.SizeOfImage;

    DWORD patternLength = (DWORD)strlen(mask);

    for (DWORD i = 0; i < size - patternLength; i++)
    {
        bool found = true;
        for (DWORD j = 0; j < patternLength; j++)
        {
            found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
        }
        if (found)
            return (base + i) + correction;
    }
    return NULL;
}

void MakeJMP(BYTE* pAddress, DWORD dwJumpTo, DWORD dwLen)
{
    DWORD dwOldProtect, dwBkup, dwRelAddr;
    // GIVE THE PAGED MEMORY READ/WRITE PERMISSIONS
    VirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);

    // CALCULATE THE DISTANCE BETWEEN OUR ADDRESS AND OUR TARGET LOCATIONS
    // AND SUBTRACT THE 5BYTES, WHICH IS THE SIZE OF A JMP
    dwRelAddr = (DWORD)(dwJumpTo - (DWORD)pAddress) - 5;

    // OVERWRITE THE BYTE AT src WITH THE JUMP OPCODE (0xE9)
    *pAddress = 0xE9;

    // OVERWRITE THE NEXT 4 BYTES (WHICH IS THE SIZE OF A POINTER)
    *((DWORD*)(pAddress + 0x1)) = dwRelAddr;

    // OVERWRITE THE REMAINING BYTES WITH NOP opcode (0x90)
    // NOP opcode = No OPeration
    for (DWORD x = 0x5; x < dwLen; x++) *(pAddress + x) = 0x90;

    // RESTORE THE PAGED MEMORY PERMISSIONS
    VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);
    return;
}

__declspec(naked) void _MidHookFunction()
{
    /*
    HeavyLogicApp.exe+17FCF - F3 0F10 05 68035400   - movss xmm0,[HeavyLogicApp.exe+20368] { (200.00) }
    HeavyLogicApp.exe+17FD7 - F3 0F11 45 DC         - movss [ebp-24],xmm0
    HeavyLogicApp.exe+17FDC - 0FB6 45 EB            - movzx eax,byte ptr [ebp-15]
    HeavyLogicApp.exe+17FE0 - 85 C0                 - test eax,eax
    HeavyLogicApp.exe+17FE2 - 74 0D                 - je HeavyLogicApp.exe+17FF1
    HeavyLogicApp.exe+17FE4 - F3 0F10 05 64035400   - movss xmm0,[HeavyLogicApp.exe+20364] { (160.00) }
    HeavyLogicApp.exe+17FEC - F3 0F11 45 DC         - movss [ebp-24],xmm0
    HeavyLogicApp.exe+17FF1 - F3 0F10 45 DC         - movss xmm0,[ebp-24]
    HeavyLogicApp.exe+17FF6 - F3 0F11 45 D0         - movss [ebp-30],xmm0
    HeavyLogicApp.exe+17FFB - F3 0F10 05 5C035400   - movss xmm0,[HeavyLogicApp.exe+2035C] { (80.00) }
    HeavyLogicApp.exe+18003 - F3 0F11 45 C4         - movss [ebp-3C],xmm0
    HeavyLogicApp.exe+18008 - F3 0F10 05 60035400   - movss xmm0,[HeavyLogicApp.exe+20360] { (120.00) }
    */

    __asm
    {
        push ebx // SET REGISTER
        mov ebx, vars

        movss xmm0, DWORD PTR[ebx]vars.customX
        movss[ebp - 36], xmm0 // sizeX

        movss xmm0, DWORD PTR[ebx]vars.customX
        movss[ebp - 48], xmm0 // sizeZup

        movss xmm0, DWORD PTR[ebx]vars.customZup
        movss[ebp - 60], xmm0 // sizeZdown

        movss xmm0, DWORD PTR[ebx]vars.customZdown
        movss[ebp - 72], xmm0 // sizeZdown

        pop ebx // RESTORE REGISTER
        JMP dwJMPback
    }
}

DWORD WINAPI MainThread(HMODULE hModule) // Our main thread
{
    vars = new Vars();

    DWORD pSig = FindPattern("HeavyLogicApp.exe", "\xF3\x0F\x11\x45\x00\x0F\xB6\x45\xEB", "xxxx?xxxx", -8);

    DWORD hookAddress = pSig;
    int sizeToJump = 0x46; // 0x46 -> 70 bytes

    dwJMPback = hookAddress + sizeToJump;

    MakeJMP((BYTE*)hookAddress, (DWORD)_MidHookFunction, sizeToJump);

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
        ourHModule = hModule;
        CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0));
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

