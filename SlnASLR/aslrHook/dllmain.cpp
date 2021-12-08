// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

#include <iostream>

HMODULE ourHModule;

using namespace std;

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

DWORD WINAPI DllThread(HMODULE hModule)
{
    // HOOK
    DWORD pSig = FindPattern("aslrApp.exe", "\x68\x00\x00\x00\x00\x8B\xDC\x68\x00\x00\x00\x00\x8B\xC4", "x????xxx????xx", 0);

    BYTE copiedBytes[5]{ 0 };
    memcpy(copiedBytes, (char*)(pSig), 5);

    const char* pSigAddress = reinterpret_cast<const char*>(copiedBytes);

    cout << "SIGNATURE Address of MyFunction: " << "0x" << uppercase << hex << pSigAddress;

    Sleep(1000);
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
        ourHModule = hModule;
        CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DllThread, hModule, 0, 0));
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

