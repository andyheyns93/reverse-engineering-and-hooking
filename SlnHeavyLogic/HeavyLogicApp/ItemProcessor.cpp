#include "ItemProcessor.h"

ItemProcessor::ItemProcessor(std::vector<Item> items)
{
	_items = items;
}
bool ItemProcessor::CheckItemsInProximity(Vec3 myPosition)
{
	srand(time(0));

	bool randomCheck = (rand() % 10) > 5;

	float sizeX = 200.f;

	if (randomCheck)
		sizeX = 160.f;

	float sizeY = sizeX;
	float sizeZup = 80.f;
	float sizeZdown = 120.f;

	AABB box(Vec3(myPosition.x - sizeX, myPosition.y - sizeY, myPosition.z - sizeZdown), Vec3(myPosition.x + sizeX, myPosition.y + sizeY, myPosition.z + sizeZup));

	bool isContaining = false;
	for (int i = 0; i < sizeof(_items); i++)
	{
		isContaining = box.IsContainPoint(_items[i].position);
		if (isContaining)
			break;
	}
	std::cout << "sizeX: " << sizeX <<  std::endl;
	std::cout << "sizeY: " << sizeY <<  std::endl;
	std::cout << "sizeZup: " << sizeZup << std::endl;
	std::cout << "sizeZdown: " << sizeZdown << std::endl;

	return isContaining;
}