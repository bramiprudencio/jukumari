#include <windows.h>
#include <psapi.h>
#include <stdio.h>

#include "hooks/processes.h"

#pragma comment(lib, "psapi.lib")

// Callback function to enumerate windows
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);

    char windowTitle[256] = { 0 };
    GetWindowText(hwnd, windowTitle, sizeof(windowTitle));

    if (strlen(windowTitle) > 0) {
        printf("Window Title: %s (Process ID: %lu)\n", windowTitle, processId);
    }

    return TRUE; // Continue enumeration
}

// Function to enumerate running processes
void EnumerateProcesses() {
    DWORD processIds[1024], cbNeeded, processCount;
    if (!EnumProcesses(processIds, sizeof(processIds), &cbNeeded)) {
        fprintf(stderr, "Failed to enumerate processes\n");
        return;
    }

    processCount = cbNeeded / sizeof(DWORD);

    printf("Running Processes:\n");
    for (unsigned int i = 0; i < processCount; i++) {
        if (processIds[i] != 0) {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i]);
            if (hProcess) {
                char processName[MAX_PATH] = "<unknown>";
                HMODULE hMod;
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                    GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(char));
                }

                printf("Process ID: %lu, Name: %s\n", processIds[i], processName);
                CloseHandle(hProcess);
            }
        }
    }
}