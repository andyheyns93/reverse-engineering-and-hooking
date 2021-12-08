// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

void Unhook(BYTE* dst, BYTE* src, unsigned int size)
{
    DWORD oldprotect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

    memcpy(dst, src, size);
    VirtualProtect(dst, size, oldprotect, &oldprotect);
}

bool Hook(char* src, char* dst, int len)
{
    if (len < 5) return false;

    DWORD oProc;
    // GIVE THE PAGED MEMORY READ/WRITE PERMISSIONS
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oProc);
    memset(src, 0x90, len);

    // CALCULATE THE DISTANCE BETWEEN OUR ADDRESS AND OUR TARGET LOCATIONS
    // AND SUBTRACT THE 5BYTES, WHICH IS THE SIZE OF A JMP
    uintptr_t relAddy = (uintptr_t)(dst - src - 5);

    // OVERWRITE THE BYTE AT src WITH THE JUMP OPCODE (0xE9)
    *src = (char)0xE9;

    // OVERWRITE THE NEXT 4 BYTES (WHICH IS THE SIZE OF A POINTER)
    *(uintptr_t*)(src + 1) = (uintptr_t)relAddy;

    // RESTORE THE PAGED MEMORY PERMISSIONS
    VirtualProtect(src, len, oProc, &oProc);

    return true;
}

char* TrampHook(char* src, char* dst, unsigned int len)
{
    if (len < 5) return 0;

    // CREATE GATEWAY
    char* gateway = (char*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    // WRITE THE STOLEN BYTES TO THE GATEWAY
    memcpy(gateway, src, len);

    // GET THE GATEWAY TO DESTINATION ADDRESS
    uintptr_t jumpAddy = (uintptr_t)(src - gateway - 5);

    // ADD THE 'JMP' OPCODE TO THE END OF THE GATEWAY
    *(gateway + len) = (char)0xE9;

    // WRITE THE ADDRESS OF THE GATEWAY TO THE JMP
    *(uintptr_t*)(gateway + len + 1) = jumpAddy;

    // PERFORM DETOUR -> WILL REPLACE THE FUNCTION CALL TO OUR FUNCTION
    if (Hook(src, dst, len))
        return gateway;

    return nullptr;
}

BYTE ThisCalculatorAdditionBytes[9]{ 0 };
typedef float(__thiscall* tThisCalculatorAddition) (DWORD*);
tThisCalculatorAddition oThisCalculatorAddition = nullptr;

float __fastcall hkThisCalculatorAddition(DWORD* thisptr, void* _notUsed)
{
    *(float*)(thisptr + 0) = 5;
    *(float*)(thisptr + 1) = 20;
    return oThisCalculatorAddition(thisptr);
}

BYTE DeclCalculatorAdditionBytes[9]{ 0 };
typedef float(__stdcall* tDeclCalculatorAddition) ( float, float);
tDeclCalculatorAddition oDeclCalculatorAddition = nullptr;

float __stdcall hkDeclCalculatorAddition(float arg1, float arg2)
{
    arg1 = arg1 * 2;
    arg2 = arg2 / 4;
    return oDeclCalculatorAddition(arg1, arg2);
}


DWORD WINAPI DllThread(HMODULE hModule)
{
    // HOOK
    uintptr_t thisCalculatorGetAdditionAddress = (0x3886F0);
    memcpy(ThisCalculatorAdditionBytes, (char*)thisCalculatorGetAdditionAddress, 9);
    oThisCalculatorAddition = (tThisCalculatorAddition)TrampHook((char*)thisCalculatorGetAdditionAddress, (char*)hkThisCalculatorAddition, 9);

    uintptr_t declCalculatorGetAdditionAddress = (0x3832B0);
    memcpy(DeclCalculatorAdditionBytes, (char*)declCalculatorGetAdditionAddress, 9);
    oDeclCalculatorAddition = (tDeclCalculatorAddition)TrampHook((char*)declCalculatorGetAdditionAddress, (char*)hkDeclCalculatorAddition, 9);

    // HOW TO HOOK VIRTUAL FUNCTIONS?
    // SAME LOGIC -> GET POINTER  + INDEX
    // DWORD* vTable = (DWORD*)(*(DWORD*)0x00000000);
    // uintptr_t vTableAddress = *(vTable + INDEX); // -> ADDRESS TO HOOK (manually look op bytes to overwrite)

    while (!GetAsyncKeyState(VK_END))
    {

    }

    // UNHOOK
    Unhook((BYTE*)thisCalculatorGetAdditionAddress, ThisCalculatorAdditionBytes, 9);
    Unhook((BYTE*)thisCalculatorGetAdditionAddress, DeclCalculatorAdditionBytes, 9);

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
        CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DllThread, hModule, 0, 0));
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

