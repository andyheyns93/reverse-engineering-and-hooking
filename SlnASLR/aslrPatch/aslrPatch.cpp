#include <Windows.h>
#include <ImageHlp.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "ImageHlp.lib")

bool PatchExecutable(LPCSTR path, bool setASLR)
{
	LOADED_IMAGE PE;
	if (MapAndLoad(path, 0, &PE, 0, 0))
	{
		if (setASLR) // Enable address space layout randomization
			PE.FileHeader->OptionalHeader.DllCharacteristics = IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE;
		else // Disable address space layout randomization
			PE.FileHeader->OptionalHeader.DllCharacteristics = NULL;
		UnMapAndLoad(&PE);
		return true;
	}
	return false;
}

int main()
{
	PatchExecutable("aslrApp.exe", false);
}
