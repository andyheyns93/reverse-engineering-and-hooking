// ConsoleAppCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DeclCalculator.h"
#include "ThisCalculator.h"

using namespace std;

int main()
{
    string strArg1;
    string strArg2;

    DeclCalculator* pDeclCalc = new DeclCalculator();
    cout << "First number: ";
    cin >> strArg1;

    cout << "Second number: ";
    cin >> strArg2;
    cout << endl;

    float arg1 = std::atof(strArg1.c_str());
    float arg2 = std::atof(strArg2.c_str());

    float addition = pDeclCalc->GetAddition(arg1, arg2);
    float division = pDeclCalc->GetDivision(arg1, arg2);
    float multiplication = pDeclCalc->GetMultiplication(arg1, arg2);
    float subtraction = pDeclCalc->GetSubtraction(arg1, arg2);

    cout << "Addition: " << addition << endl;
    cout << "Division: " << division << endl;
    cout << "Subtraction: " << subtraction << endl;
    cout << "Multiplication: " << multiplication << endl;

    ThisCalculator* pThisCalc = new ThisCalculator(arg1, arg2);

    addition = pThisCalc->GetAddition();
    division = pThisCalc->GetDivision();
    multiplication = pThisCalc->GetMultiplication();
    subtraction = pThisCalc->GetSubtraction();

    cout << "Addition: " << addition << endl;
    cout << "Division: " << division << endl;
    cout << "Subtraction: " << subtraction << endl;
    cout << "Multiplication: " << multiplication << endl;

    system("pause");
}
