// ConsoleApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>

struct Test {
    int a = 1;
};

int main()
{
    while (!(GetAsyncKeyState(VK_END)))
    {
    }

    float a1 = 5.5f;
    float a2 = 6.5f;
    Test* a = new Test();

    float b1 = a1;
    float b2 = a2;

    std::cout << "b1: " << b1 << " - b2: " << b2 << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
