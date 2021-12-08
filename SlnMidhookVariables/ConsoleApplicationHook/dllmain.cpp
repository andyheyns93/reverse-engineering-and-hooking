// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

HMODULE ourHModule;
DWORD dwJMPback = 0;

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

struct Var {
    int i;
};

Var* pVar;
float b1;
float b2;

void ExecuteFunction(float a1, float a2, Var* pVar)
{

}

__declspec(naked) void _MidHookFunction()
{
    __asm
    {
        mov edx, [ebp - 268] // overriden instruction

        movss xmm0, [ebp - 8]
        movss b1, xmm0

        movss xmm0, [ebp - 20]
        movss b2, xmm0

        push ebx
        mov ebx, [ebp - 260]
        mov pVar, ebx
        pop ebx

        JMP dwJMPback
    }
    ExecuteFunction(b1, b2, pVar);
    __asm
    {
        JMP dwJMPback
    }
}

DWORD WINAPI MainThread(HMODULE hModule) // Our main thread
{
    DWORD hookAddress = (0x00612757);
    int sizeToJump = 0x6;

    dwJMPback = hookAddress + sizeToJump;

    MakeJMP((BYTE*)hookAddress, (DWORD)_MidHookFunction, sizeToJump);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
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