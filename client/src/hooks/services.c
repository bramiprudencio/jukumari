#include <windows.h>
#include <stdio.h>

#include "hooks/services.h"

void PrintRunningServices() {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
    if (!hSCManager) {
        fprintf(stderr, "Failed to open Service Control Manager: %lu\n", GetLastError());
        return;
    }

    DWORD bytesNeeded, servicesReturned, resumeHandle = 0;
    DWORD bufferSize = 0;
    LPBYTE buffer = NULL;

    // Determine the required buffer size
    EnumServicesStatusEx(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_ACTIVE,
        NULL,
        0,
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL
    );

    if (GetLastError() != ERROR_MORE_DATA) {
        fprintf(stderr, "Failed to enumerate services: %lu\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return;
    }

    // Allocate the buffer
    bufferSize = bytesNeeded;
    buffer = (LPBYTE)malloc(bufferSize);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory\n");
        CloseServiceHandle(hSCManager);
        return;
    }

    // Enumerate services
    if (!EnumServicesStatusEx(
        hSCManager,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_ACTIVE,
        buffer,
        bufferSize,
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL)) {
        fprintf(stderr, "Failed to enumerate services: %lu\n", GetLastError());
        free(buffer);
        CloseServiceHandle(hSCManager);
        return;
    }

    // Cast buffer to ENUM_SERVICE_STATUS_PROCESS array
    ENUM_SERVICE_STATUS_PROCESS* services = (ENUM_SERVICE_STATUS_PROCESS*)buffer;

    printf("Running Services:\n");
    for (DWORD i = 0; i < servicesReturned; i++) {
        printf("Service Name: %s\n", services[i].lpServiceName);
        printf("Display Name: %s\n", services[i].lpDisplayName);
        printf("Process ID: %lu\n\n", services[i].ServiceStatusProcess.dwProcessId);
    }

    // Cleanup
    free(buffer);
    CloseServiceHandle(hSCManager);
}