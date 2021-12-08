#include <iostream>

int __cdecl function_cdecl(int a, int b, int c) {
    return (a + b + c);
}
int __stdcall function_standard(int a, int b, int c) {
    return (a + b + c);
}
int __fastcall function_fast(int a, int b, int c) {
    return (a + b + c);
}
int main()
{
    function_cdecl(1, 2, 3);
    function_standard(1, 2, 3);
    function_fast(1, 2, 3);

    system("pause");
    return 0;
}
