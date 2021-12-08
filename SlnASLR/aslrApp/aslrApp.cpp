// aslrApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>

using namespace std;

void MyFunction(int x)
{
    cout << "Address of x: " << "0x" << uppercase << hex << &x;
}

int main()
{
    int i = 50;
    //prints the location of i, changes every time if ASLR == enabled
    cout << "Address of I: " << "0x" << uppercase << hex << &i << endl;

    void (*fnFunc)(int);
    fnFunc = &MyFunction;

    cout << "Static  Address of MyFunction: " << "0x" << uppercase << hex << &MyFunction << endl;
    cout << "Dynamic Address of MyFunction: " << "0x" << uppercase << hex << &fnFunc << endl;

    fnFunc(2);

    system("pause");
}

