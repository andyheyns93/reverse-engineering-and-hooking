#include "windows.h"
#include <iostream>

struct DataTableParent {
    char padx0000[4];
    std::string firstName;
    std::string lastName;
};

struct DataTableChild {
    char padx0000[4];
    std::string firstName;
    std::string lastName;
    int age;
    int year;
};

struct IParent {
    virtual void PrintFullName() = 0;
    virtual void PrintExtendedFullName() = 0;
};

class Parent : public IParent {
public:
    std::string firstName;
    std::string lastName;

    Parent(std::string firstName, std::string lastName)
    {
        this->firstName = firstName;
        this->lastName = lastName;
    }

    virtual void PrintFullName()
    {
        std::cout << firstName << " " << lastName << std::endl;
    }
    virtual void PrintExtendedFullName()
    {
        std::cout << "EXTENDED: " << firstName << " " << lastName << std::endl;
    }
};

class Child: public Parent {
public:

    int age;
    int year;

    Child(std::string firstName, std::string lastName, int age, int year) :Parent(firstName, lastName) {
        this->age = age;
        this->year = year;
    }

    virtual void PrintFullName()
    {
        std::cout << firstName << " " << lastName << std::endl;
    }
    virtual void PrintExtendedFullName()
    {
        std::cout << "EXTENDED: " << firstName << " " << lastName << std::endl;
    }
};

template <typename t>
t VIRTUAL(PVOID BASE, DWORD32 INDEX)
{
    t* pBase = (*(t**)BASE);
    if (pBase) {
        return pBase[INDEX];
    }
    return NULL;
}

int main()
{
    Parent* pParent = new Parent("John", "Doe");
    Child* pChild = new Child("Jane", "Does", 18, 1990);

    VIRTUAL<void(__thiscall*)(PVOID)>(pParent, 0)(pParent);
    VIRTUAL<void(__thiscall*)(PVOID)>(pParent, 1)(pParent);

    VIRTUAL<void(__thiscall*)(PVOID)>(pChild, 0)(pChild);
    VIRTUAL<void(__thiscall*)(PVOID)>(pChild, 1)(pChild);

    DataTableParent* pDataTableParent = (DataTableParent*)((DWORD)pParent);
    DataTableChild* pDataTableChild = (DataTableChild*)((DWORD)pChild);

    std::string parentFirstName = *(std::string*)((DWORD)pParent + 4); // START 
    std::string parentLastName = *(std::string*)((DWORD)pParent + 32); // +28  (string takes 32 bytes - 4 of current offset)

    std::string childFirstName = *(std::string*)((DWORD)pChild + 4); // START 
    std::string childLastName = *(std::string*)((DWORD)pChild + 32); // + 28 (string takes 32 bytes - 4 of current offset)
    int childAge = *(int*)((DWORD)pChild + 60); // +28  (string takes 32 bytes - 4 of current offset)
    int childYear = *(int*)((DWORD)pChild + 64); // +4 (int takes 4 bytes)

    system("pause");
}
