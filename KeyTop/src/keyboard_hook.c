#include "keyboard_hook.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

// Static variables
static HHOOK keyboardHook = NULL;
static AppConfig* currentConfig = NULL;
static KeyCallback keyCallback = NULL;
static BOOL enabled = FALSE;

// Internal function declarations
static const char* GetKeyName(UINT vk);
static UINT GetVkFromName(const char* name);

// Low-level keyboard hook callback
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) {
        return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
    }
    if (!enabled || !currentConfig) {
        return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
    }
    KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam;
    if (!pkbhs) {
        return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
    }
    UINT vk = pkbhs->vkCode;
    BOOL keyDown = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
    if (keyCallback) {
        keyCallback(vk, keyDown);
    }
    for (int i = 0; i < currentConfig->mappingCount; i++) {
        KeyMapping* mapping = &currentConfig->mappings[i];
        UINT fromVk = GetVkFromName(mapping->fromKey);
        UINT toVk = GetVkFromName(mapping->toKey);
        if (fromVk == vk) {
            if (keyDown) {
                keybd_event(toVk, 0, 0, 0);
            } else {
                keybd_event(toVk, 0, KEYEVENTF_KEYUP, 0);
            }
            return 1;
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

// Get key name from VK code (for logging)
static const char* GetKeyName(UINT vk) {
    static char buffer[MAX_KEY_NAME_LENGTH];
    switch (vk) {
        case VK_BACK: return "Backspace";
        case VK_TAB: return "Tab";
        case VK_RETURN: return "Enter";
        case VK_PAUSE: return "Pause";
        case VK_CAPITAL: return "CapsLock";
        case VK_ESCAPE: return "Escape";
        case VK_SPACE: return "Space";
        case VK_PRIOR: return "PageUp";
        case VK_NEXT: return "PageDown";
        case VK_END: return "End";
        case VK_HOME: return "Home";
        case VK_LEFT: return "Left";
        case VK_UP: return "Up";
        case VK_RIGHT: return "Right";
        case VK_DOWN: return "Down";
        case VK_SNAPSHOT: return "PrintScreen";
        case VK_INSERT: return "Insert";
        case VK_DELETE: return "Delete";
        case VK_NUMPAD0: return "Num0";
        case VK_NUMPAD1: return "Num1";
        case VK_NUMPAD2: return "Num2";
        case VK_NUMPAD3: return "Num3";
        case VK_NUMPAD4: return "Num4";
        case VK_NUMPAD5: return "Num5";
        case VK_NUMPAD6: return "Num6";
        case VK_NUMPAD7: return "Num7";
        case VK_NUMPAD8: return "Num8";
        case VK_NUMPAD9: return "Num9";
        case VK_MULTIPLY: return "Num*";
        case VK_ADD: return "Num+";
        case VK_SUBTRACT: return "Num-";
        case VK_DECIMAL: return "Num.";
        case VK_DIVIDE: return "Num/";
        case VK_F1: return "F1";
        case VK_F2: return "F2";
        case VK_F3: return "F3";
        case VK_F4: return "F4";
        case VK_F5: return "F5";
        case VK_F6: return "F6";
        case VK_F7: return "F7";
        case VK_F8: return "F8";
        case VK_F9: return "F9";
        case VK_F10: return "F10";
        case VK_F11: return "F11";
        case VK_F12: return "F12";
        case VK_NUMLOCK: return "NumLock";
        case VK_SCROLL: return "ScrollLock";
        case VK_LSHIFT: return "LShift";
        case VK_RSHIFT: return "RShift";
        case VK_LCONTROL: return "LCtrl";
        case VK_RCONTROL: return "RCtrl";
        case VK_LMENU: return "LAlt";
        case VK_RMENU: return "RAlt";
        default:
            if (vk >= 'A' && vk <= 'Z') {
                snprintf(buffer, sizeof(buffer), "%c", (char)vk);
                return buffer;
            }
            if (vk >= '0' && vk <= '9') {
                snprintf(buffer, sizeof(buffer), "%c", (char)vk);
                return buffer;
            }
            {
                LONG lParam = MapVirtualKey(vk, MAPVK_VK_TO_VSC) << 16;
                if (GetKeyNameText(lParam, buffer, sizeof(buffer)) > 0) {
                    return buffer;
                }
            }
            snprintf(buffer, sizeof(buffer), "VK_%02X", vk);
            return buffer;
    }
}

// Get VK code from key name (same implementation as in config.c)
static UINT GetVkFromName(const char* name) {
    if (!name || !name[0]) return 0;
    if (stricmp(name, "Backspace") == 0) return VK_BACK;
    if (stricmp(name, "Tab") == 0) return VK_TAB;
    if (stricmp(name, "Enter") == 0) return VK_RETURN;
    if (stricmp(name, "Pause") == 0) return VK_PAUSE;
    if (stricmp(name, "CapsLock") == 0) return VK_CAPITAL;
    if (stricmp(name, "Escape") == 0) return VK_ESCAPE;
    if (stricmp(name, "Space") == 0) return VK_SPACE;
    if (stricmp(name, "PageUp") == 0) return VK_PRIOR;
    if (stricmp(name, "PageDown") == 0) return VK_NEXT;
    if (stricmp(name, "End") == 0) return VK_END;
    if (stricmp(name, "Home") == 0) return VK_HOME;
    if (stricmp(name, "Left") == 0) return VK_LEFT;
    if (stricmp(name, "Up") == 0) return VK_UP;
    if (stricmp(name, "Right") == 0) return VK_RIGHT;
    if (stricmp(name, "Down") == 0) return VK_DOWN;
    if (stricmp(name, "PrintScreen") == 0) return VK_SNAPSHOT;
    if (stricmp(name, "Insert") == 0) return VK_INSERT;
    if (stricmp(name, "Delete") == 0) return VK_DELETE;
    if (stricmp(name, "Num0") == 0) return VK_NUMPAD0;
    if (stricmp(name, "Num1") == 0) return VK_NUMPAD1;
    if (stricmp(name, "Num2") == 0) return VK_NUMPAD2;
    if (stricmp(name, "Num3") == 0) return VK_NUMPAD3;
    if (stricmp(name, "Num4") == 0) return VK_NUMPAD4;
    if (stricmp(name, "Num5") == 0) return VK_NUMPAD5;
    if (stricmp(name, "Num6") == 0) return VK_NUMPAD6;
    if (stricmp(name, "Num7") == 0) return VK_NUMPAD7;
    if (stricmp(name, "Num8") == 0) return VK_NUMPAD8;
    if (stricmp(name, "Num9") == 0) return VK_NUMPAD9;
    if (stricmp(name, "Num*") == 0) return VK_MULTIPLY;
    if (stricmp(name, "Num+") == 0) return VK_ADD;
    if (stricmp(name, "Num-") == 0) return VK_SUBTRACT;
    if (stricmp(name, "Num.") == 0) return VK_DECIMAL;
    if (stricmp(name, "Num/") == 0) return VK_DIVIDE;
    if (stricmp(name, "F1") == 0) return VK_F1;
    if (stricmp(name, "F2") == 0) return VK_F2;
    if (stricmp(name, "F3") == 0) return VK_F3;
    if (stricmp(name, "F4") == 0) return VK_F4;
    if (stricmp(name, "F5") == 0) return VK_F5;
    if (stricmp(name, "F6") == 0) return VK_F6;
    if (stricmp(name, "F7") == 0) return VK_F7;
    if (stricmp(name, "F8") == 0) return VK_F8;
    if (stricmp(name, "F9") == 0) return VK_F9;
    if (stricmp(name, "F10") == 0) return VK_F10;
    if (stricmp(name, "F11") == 0) return VK_F11;
    if (stricmp(name, "F12") == 0) return VK_F12;
    if (stricmp(name, "NumLock") == 0) return VK_NUMLOCK;
    if (stricmp(name, "ScrollLock") == 0) return VK_SCROLL;
    if (stricmp(name, "LShift") == 0) return VK_LSHIFT;
    if (stricmp(name, "RShift") == 0) return VK_RSHIFT;
    if (stricmp(name, "LCtrl") == 0) return VK_LCONTROL;
    if (stricmp(name, "RCtrl") == 0) return VK_RCONTROL;
    if (stricmp(name, "LAlt") == 0) return VK_LMENU;
    if (stricmp(name, "RAlt") == 0) return VK_RMENU;
    if (strlen(name) == 1) {
        char c = name[0];
        if (c >= 'A' && c <= 'Z') return c;
        if (c >= 'a' && c <= 'z') return toupper(c);
        if (c >= '0' && c <= '9') return c;
    }
    for (UINT vk = 0; vk < 256; vk++) {
        const char* keyName = GetKeyName(vk);
        if (stricmp(name, keyName) == 0) {
            return vk;
        }
    }
    return 0;
}

// Initialize the keyboard hook
BOOL KeyboardHookInit(AppConfig* config) {
    if (!config) {
        LogMessage(LOG_ERROR, "KeyboardHook", "NULL config passed to KeyboardHookInit");
        return FALSE;
    }
    if (enabled && keyboardHook) {
        LogMessage(LOG_WARNING, "KeyboardHook", "Keyboard hook already initialized, shutting down first");
        KeyboardHookShutdown();
    }
    currentConfig = config;
    enabled = TRUE;
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, GetModuleHandle(NULL), 0);
    if (!keyboardHook) {
        LogMessage(LOG_ERROR, "KeyboardHook", "Failed to set keyboard hook (error: %lu)", GetLastError());
        enabled = FALSE;
        currentConfig = NULL;
        return FALSE;
    }
    LogMessage(LOG_INFO, "KeyboardHook", "Keyboard hook initialized successfully");
    return TRUE;
}

// Shutdown the keyboard hook
void KeyboardHookShutdown(void) {
    if (keyboardHook) {
        if (!UnhookWindowsHookEx(keyboardHook)) {
            LogMessage(LOG_ERROR, "KeyboardHook", "Failed to unhook keyboard (error: %lu)", GetLastError());
        } else {
            LogMessage(LOG_INFO, "KeyboardHook", "Keyboard hook shut down successfully");
        }
        keyboardHook = NULL;
    }
    enabled = FALSE;
    currentConfig = NULL;
    keyCallback = NULL;
}

// Reload configuration
void KeyboardHookReloadConfig(AppConfig* config) {
    if (!config) {
        LogMessage(LOG_ERROR, "KeyboardHook", "NULL config passed to KeyboardHookReloadConfig");
        return;
    }
    currentConfig = config;
    LogMessage(LOG_INFO, "KeyboardHook", "Configuration reloaded (mappings: %d)", config->mappingCount);
}

// Set key callback function
void KeyboardHookSetCallback(KeyCallback callback) {
    keyCallback = callback;
    LogMessage(LOG_DEBUG, "KeyboardHook", "Key callback %s", callback ? "set" : "cleared");
}

// Check if keyboard hook is enabled
BOOL KeyboardHookIsEnabled(void) {
    return enabled && keyboardHook != NULL;
}
