#include <Windows.h>
#include <iostream>

using namespace std;

class Person;

Person* p = nullptr;

struct IPerson {
	virtual void Print() = 0;
};

class Person : public IPerson {
public
	:
		string m_firstName;
		string m_LastName;

		Person(string firstName, string lastName)
		{
			this->m_firstName = firstName;
			this->m_LastName = lastName;
		}

		void PrintFirstName()
		{
			cout << "FirstName: " << m_firstName << endl;
		}
		
		void PrintLastName()
		{
			cout << "LastName: " << m_LastName << endl;
		}

		void Print()
		{
			cout << "FirstName: " << m_firstName << " - LastName: " << m_LastName << endl;
		}
};

void FunctionCallWithoutParam()
{
	cout << "FunctionCallWithoutParam" << endl;
}

void FunctionCallWithParam(string textToPrint)
{
	cout << textToPrint << endl;
}


int main()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			FunctionCallWithoutParam();
			FunctionCallWithParam("FunctionCallWithParam");
		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			p = new Person("John", "Doe");
			p->PrintFirstName();
			p->PrintLastName();
			p->Print();
		}
	}
	return 0;
}
