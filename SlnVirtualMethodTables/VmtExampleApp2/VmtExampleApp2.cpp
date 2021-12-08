#include "windows.h"
#include <iostream>

using namespace std;

class B1 {
public:
    int int_in_b1 = 5;

    virtual ~B1()
    {
        cout << "DESTRUCTOR B1" << std::endl;
    }
    void f0() {
        cout << "B1::F0" << std::endl;
    }
    virtual void f1()
    {
        cout << "B1::F1" << std::endl;
    }
};

class B2 {
public:
    int int_in_b2 = 10;

    virtual ~B2()
    {
        cout << "DESTRUCTOR B2" << std::endl;
    }
    virtual void f2()
    {
        cout << "B2::F2" << std::endl;
    }
};

class D : public B1, public B2 {
public:
    int int_in_d = 15;

    void d()
    {
        cout << "CONSTRUCTOR D" << std::endl;
    }
    void f2() override
    {
        cout << "D:F2 (override B2::F2)" << std::endl;
    }
};

template <typename t>
t VIRTUAL(PVOID BASE, DWORD32 INDEX, DWORD32 vmtIndex = 0)
{
    t* pBase = (*(t**)BASE + (vmtIndex * 4));
    if (pBase) {
        return pBase[INDEX];
    }
    return NULL;
}

void fnCall();
bool (*funcPtr)();

int main()
{
    B1* b1 = new B1();
    B2* b2 = new B2();
    D* d = new D(); // +0 = B1 / +4 = B2

    VIRTUAL<void(__thiscall*)(PVOID)>(b1, 1)(b1);
    VIRTUAL<void(__thiscall*)(PVOID)>(b2, 1)(b2);
    VIRTUAL<void(__thiscall*)(PVOID)>(d, 1)(d);

    VIRTUAL<void(__thiscall*)(PVOID)>(d, 1, 0)(d);
    VIRTUAL<void(__thiscall*)(PVOID)>(d, 1, 1)(d);

    int b1_int = *(int*)((DWORD)b1 + 4);
    int b2_int = *(int*)((DWORD)b2 + 4);
    int d_int = *(int*)((DWORD)d + 4);
    int d_int2 = *(int*)((DWORD)d + 12);
    int d_int3 = *(int*)((DWORD)d + 16);

    D* pD8 = (D*)((DWORD)d + 8); //WHAT HOLDS POSITION +8 ? ->  D:F2 (override B2::F2)
    VIRTUAL<void(__thiscall*)(PVOID)>(pD8, 1)(pD8);

    cout << "d_int: " << d_int << endl;
    cout << "d_int2: " << d_int2 << endl;
    cout << "d_int3: " << d_int3 << endl;

    system("pause");
}

