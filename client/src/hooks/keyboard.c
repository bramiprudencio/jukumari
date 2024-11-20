#include <stdio.h>
#include <windows.h>

#include "common.h"
#include "hooks/keyboard.h"

static HHOOK hKeyboardHook;

// Hook procedure for the keyboard
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* kbStruct = (KBDLLHOOKSTRUCT*)lParam;

        // Check if the event is a key down event
        if (wParam == WM_KEYDOWN) {
            printf("Key Pressed: %c\n", VkCodeToChar(kbStruct->vkCode));
        }
    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

// Function to set the hook
void SetKeyboardHook() {
    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);
    if (hKeyboardHook == NULL) {
        printf("Failed to set keyboad hook\n");
    }
}

// Function to remove the hook
void RemoveKeyboardHook() {
    UnhookWindowsHookEx(hKeyboardHook);
}
