#include <windows.h>
#include <stdio.h>

HHOOK hKeyboardHook;

// Hook procedure for the keyboard
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* kbStruct = (KBDLLHOOKSTRUCT*)lParam;

        // Check if the event is a key down event
        if (wParam == WM_KEYDOWN) {
            printf("Key Pressed: %d\n", kbStruct->vkCode);
        }
    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

// Function to set the hook
void SetHook() {
    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);
    if (hKeyboardHook == NULL) {
        printf("Failed to set hook\n");
    }
}

// Function to remove the hook
void RemoveHook() {
    UnhookWindowsHookEx(hKeyboardHook);
}

int main() {
    // Set the keyboard hook
    SetHook();

    // Message loop to keep the hook active
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Remove the hook when done
    RemoveHook();
    return 0;
}
