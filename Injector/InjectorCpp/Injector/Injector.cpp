#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <Shlwapi.h>

DWORD GetProcessId(const char* processName)
{
    DWORD processId = 0;
    HANDLE handleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (handleSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(processEntry);

        if (Process32First(handleSnapshot, &processEntry))
        {
            do
            {
                if (!_stricmp(processEntry.szExeFile, processName))
                {
                    processId = processEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(handleSnapshot, &processEntry));
        }
    }
    CloseHandle(handleSnapshot);
    return processId;
}

std::string ExtractDirectory(const std::string& path)
{
    return path.substr(0, path.find_last_of('\\') + 1);
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Amount of args:" << argc << std::endl;
        for (size_t i = 0; i < sizeof(argv) -1 ; i++)
        {
            std::cerr << "Arg" << i << " " << argv[i] << std::endl;
        }
        std::cerr << "We Expect 2 arguments > injector.exe <dllname> <exenametoinject>" << std::endl;
        return 1;
    }

    std::cerr << "Dll Name:" << argv[1] << std::endl;
    std::cerr << "Exe Name:" << argv[2] << std::endl;

    system("pause");

    char ownPth[MAX_PATH];
    HMODULE hModule = GetModuleHandle(NULL);
    GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));

    std::string directory = ExtractDirectory(std::string(ownPth));
    std::string dllName(argv[1]); //dll name
    dllName.insert(0, directory);

    const char* dllInject = const_cast<char*>(dllName.c_str());
    const char* processName = argv[2]; //process name
    DWORD processId = 0;

    while (!processId)
    {
        processId = GetProcessId(processName);
    }

    HANDLE handleProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);

    if (handleProcess && handleProcess != INVALID_HANDLE_VALUE)
    {
        void* virtualAlloc = VirtualAllocEx(handleProcess, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (virtualAlloc)
        {
            WriteProcessMemory(handleProcess, virtualAlloc, dllInject, strlen(dllInject) + 1, 0);
        }

        HANDLE handleThread = CreateRemoteThread(handleProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, virtualAlloc, 0, 0);

        if (handleThread) {
            CloseHandle(handleThread);
        }
    }
    if (handleProcess) {
        CloseHandle(handleProcess);
    }

    system("pause");
    return 0;
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
