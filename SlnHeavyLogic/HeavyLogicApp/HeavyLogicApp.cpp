// HeavyLogicApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <vector>

#include "ItemProcessor.h"
#include "Item.h"

using namespace std;

void generate_rand(float& x, float& y, float& z)
{
    float offset = 999.999999f;
    do {
        //x = 1 + rand() % 999;
        x = ((float)rand() / (float)(RAND_MAX) * offset);
    } while (x == 0);

    do {
        //y = 1 + rand() % 999;
        y = ((float)rand() / (float)(RAND_MAX) * offset );
    } while (y == x);

    do {
        //z = 1 + rand() % 999;
        z = ((float)rand() / (float)(RAND_MAX)*offset);
    } while (z == x || z == y);
}

std::vector<Item> GetItemSeed(int size)
{
    srand(time(0));

    std::vector<Item> itemList;
    for (int i = 0; i < size; i++)
    {
        float x;
        float y;
        float z;
        generate_rand(x, y, z);

        Vec3 pos(x, y, z);
        Item item(pos);

        itemList.push_back(item);
    }
    return itemList;
}

int main()
{

    std::vector<Item> seed = GetItemSeed(1000000);

    Vec3 myPosition;
    generate_rand(myPosition.x, myPosition.y, myPosition.z);

    ItemProcessor* pItemProcessor = new ItemProcessor(seed);

    while (!GetAsyncKeyState(VK_END))
    {
        Sleep(100);
    }

    if (pItemProcessor->CheckItemsInProximity(myPosition))
    {
        std::cout << "Has Items In Proximity" << std::endl;
    }
    else
    {
        std::cout << "No Items In Proximity" << std::endl;
    }

    system("pause");
}
