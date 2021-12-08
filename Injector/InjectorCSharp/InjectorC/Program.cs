using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace InjectorC
{
    class Program
    {
        [DllImport("kernel32.dll")]
        internal static extern IntPtr OpenProcess(uint dwDesiredAccess, int bInheritHandle, int dwProcessId);

        [DllImport("kernel32.dll")]
        internal static extern IntPtr GetProcAddress(IntPtr hModule, string lpProcName);

        [DllImport("kernel32.dll")]
        internal static extern IntPtr GetModuleHandle(string lpModuleName);

        [DllImport("kernel32.dll")]
        internal static extern int CloseHandle(IntPtr hObject);

        [DllImport("kernel32.dll")]
        internal static extern IntPtr VirtualAllocEx(IntPtr hProcess, IntPtr dllMemoryPointer, IntPtr dwSize, uint flAllocationType, uint flProtect);

        [DllImport("kernel32.dll")]
        internal static extern void VirtualFreeEx(IntPtr hProcess, IntPtr lpAddress, int dwSize, uint dwFreeType);

        [DllImport("kernel32.dll")]
        internal static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] buffer, uint size, out UIntPtr lpNumberOfBytesWritte);

        [DllImport("kernel32.dll")]
        internal static extern IntPtr CreateRemoteThread(IntPtr hProcess, IntPtr lpThreadAttribute, uint dwStackSize, IntPtr lpStartAddress,
            IntPtr lpParameter, uint dwCreationFlags, IntPtr lpThreadId);

        [DllImport("kernel32.dll")]
        internal static extern void WaitForSingleObject(IntPtr hHandle, uint dwMilliseconds);

        const int PROCESS_CREATE_THREAD = 0x2;
        const int PROCESS_QUERY_INFORMATION = 0x400;
        const int PROCESS_VM_OPERATION = 0x8;
        const int PROCESS_VM_WRITE = 0x20;
        const int PROCESS_VM_READ = 0x10;

        const int MEM_COMMIT = 0x1000;
        const int MEM_RESERVE = 0x2000;
        const int PAGE_READWRITE = 0x4;

        public static Process GetProcessByName(string process)
        {
            var processName = process.Split('.')[0];
            var foundProcess = Process.GetProcessesByName(processName).FirstOrDefault();

            if (foundProcess is not null && foundProcess.MainModule.ModuleName.Equals(process, StringComparison.InvariantCultureIgnoreCase))
            {
                if (!string.IsNullOrEmpty(foundProcess.MainWindowTitle))
                    Console.WriteLine($"Process: {foundProcess.ProcessName} ID: {foundProcess.Id} Window title: {foundProcess.MainWindowTitle}");

                return foundProcess;
            }

            throw new ArgumentOutOfRangeException($"Process not found with name: {process}");
        }

        public static bool InjectDLL(string dllPath, string processName)
        {
            Process proc = GetProcessByName(processName);

            if (proc is null)
                return false;

            IntPtr procHandle  = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, 1, proc.Id);

            if (procHandle  != IntPtr.Zero)
            {
                IntPtr allocMemAddress = VirtualAllocEx(procHandle , IntPtr.Zero, (IntPtr)dllPath.Length, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

                if (allocMemAddress.Equals(0))
                    return false;

                UIntPtr bytesWritten;
                byte[] dllBytes = Encoding.ASCII.GetBytes(dllPath);
                bool result = WriteProcessMemory(procHandle , allocMemAddress, dllBytes, (uint)((dllPath.Length + 1) * Marshal.SizeOf(typeof(char))), out bytesWritten);

                if (!result)
                    return false;

                IntPtr loadLibraryAddr  = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

                IntPtr hThread = CreateRemoteThread(procHandle , IntPtr.Zero, 0, loadLibraryAddr , allocMemAddress, 0, IntPtr.Zero);

                if (!hThread.Equals(0))
                    CloseHandle(hThread);
                else return false;
            }
            else return false;

            proc.Dispose();
            return true;
        }

        static void Main(string[] args)
        {
            bool result = InjectDLL(@"pathToDll.dll", "name.exe");
            Console.ReadKey();
        }
    }
}
