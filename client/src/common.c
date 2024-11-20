#include <windows.h>
#include <stdio.h>

#include "common.h"

// Function to convert vkCode to a character
char VkCodeToChar(DWORD vkCode) {
    BYTE keyboardState[256];
    if (!GetKeyboardState(keyboardState)) {
        return '\0'; // Return null if unable to get keyboard state
    }

    // Buffer to hold the translated character
    WCHAR buffer[2] = { 0 };
    HKL keyboardLayout = GetKeyboardLayout(0);

    // Convert vkCode to Unicode character
    if (ToUnicode(vkCode, MapVirtualKey(vkCode, MAPVK_VK_TO_VSC), keyboardState, buffer, 2, 0) == 1) {
        return (char)buffer[0]; // Return the first character
    }

    return '\0'; // Return null if no character could be mapped
}