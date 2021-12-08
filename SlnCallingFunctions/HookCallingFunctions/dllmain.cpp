// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>

using namespace std;

typedef void(__cdecl* _FunctionCallWithoutParam)(void);
_FunctionCallWithoutParam FunctionCallWithoutParam;

typedef void(__cdecl* _FunctionCallWithParam)(string);
_FunctionCallWithParam FunctionCallWithParam;

typedef void(__thiscall* _PrintPerson)(void*);
_PrintPerson FunctionMemberCall;

template <typename t>
t VIRTUAL(PVOID BASE, DWORD32 INDEX)
{
    t* pBase = (*(t**)BASE);
    if (pBase) {
        return pBase[INDEX];
    }
    return NULL;
}

struct MyPersonClass
{
public:
    void Print()
    {
        VIRTUAL<void(__thiscall*)(PVOID)>(this, 0)(this);
    }

    typedef int(__thiscall* _PrintFirstName)(void*);
    void PrintFirstName()
    {
        _PrintFirstName fnPrint = (_PrintFirstName)(0xE34DF0);
        fnPrint(this);
    }

    typedef int(__thiscall* _PrintLastName)(void*);
    void PrintLastName()
    {
        _PrintLastName fnPrint = (_PrintLastName)(0xE34E90);
        fnPrint(this);
    }
};

DWORD WINAPI MainThread(LPVOID hModule)
{
    // NOTE: offsets will be different each time app is build!
    FunctionCallWithoutParam = (_FunctionCallWithoutParam)(0xE34CB0);
    FunctionCallWithParam = (_FunctionCallWithParam)(0xE34BE0);
    FunctionMemberCall = (_PrintPerson)(0xE34D30);

    cout << "dll injected!" << endl;

    while (true)
    {
        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            FunctionCallWithoutParam();
        }
        if (GetAsyncKeyState(VK_NUMPAD4) & 1)
        {
            FunctionCallWithParam("Call it with function");
        }
        if (GetAsyncKeyState(VK_NUMPAD5) & 1)
        {
            void* pPerson = *(void**)(0xE40410);
            if (pPerson == nullptr) {

                cout << "Person is not initialized (use static pointer or create new instance)" << endl;
            }
            else {
                FunctionMemberCall(pPerson);
            }
        }
        if (GetAsyncKeyState(VK_NUMPAD6) & 1)
        {
            MyPersonClass* pPerson = *(MyPersonClass**)(0xE40410);
            if (pPerson == nullptr) {

                cout << "Person is not initialized (use static pointer or create new instance)" << endl;
            }
            else {
                pPerson->PrintFirstName();
                pPerson->PrintLastName();
                pPerson->Print();
            }
        }
        if (GetAsyncKeyState(VK_END) & 1)
        {
            cout << "dll ejected!" << endl;
            break;
        }
    }

    FreeLibraryAndExitThread((HMODULE)hModule, 0);
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
        CreateThread(0, 0, MainThread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

