// https://faydoc.tripod.com/cpu/jz.htm

#include <iostream>

const char usable[] = "0123456789!@#$%^&*abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

using namespace std;

string GetRandomPassword(int size)
{
    srand(time(0));

    string pass = "";
    for (int i = 0; i < size; i++)
    {
        int random = rand() % (sizeof(usable) -1);
        char randomUse = usable[random];
        pass.append(1, randomUse);
    }
    return pass;
}

int main()
{
    string secretPassword = GetRandomPassword(16);
    string passwordToCheck;

    cout << "Generated password: " << secretPassword << endl;
    int count = 0;
    while (passwordToCheck != secretPassword)
    {
        if (count != 0)
            cout << "Wrong Password!" << endl;
        cout << "Type in password:";
        cin >> passwordToCheck;
        count++;
    }

    cout << "Correct Password!" << endl;

    system("pause");
}

