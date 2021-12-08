#pragma once
#include <iostream>
#include <vector>
#include <time.h>

#include "Item.h"
#include "AABB.h"
#include "Vec3.h"

class ItemProcessor
{
private:
	std::vector<Item> _items;
public:
	ItemProcessor(std::vector<Item> items);
	bool CheckItemsInProximity(Vec3 myPosition);
};

