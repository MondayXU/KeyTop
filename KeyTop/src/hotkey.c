#include "hotkey.h"
#include "utils.h"

static HotkeyCallback hotkeyCallback = NULL;
static HINSTANCE appInstance = NULL;

BOOL HotkeyInit(HINSTANCE hInstance)
{
    appInstance = hInstance;
    LogMessage(LOG_INFO, "Hotkey", "Hotkey module initialized");
    return TRUE;
}

void HotkeyShutdown(void)
{
    LogMessage(LOG_INFO, "Hotkey", "Hotkey module shutdown");
    hotkeyCallback = NULL;
    appInstance = NULL;
}

BOOL HotkeyRegister(int id, UINT modifiers, UINT vk)
{
    if (!appInstance) {
        LogMessage(LOG_ERROR, "Hotkey", "Cannot register hotkey - module not initialized");
        return FALSE;
    }
    
    if (!RegisterHotKey(NULL, id, modifiers, vk)) {
        LogMessage(LOG_ERROR, "Hotkey", "Failed to register hotkey id %d", id);
        return FALSE;
    }
    
    LogMessage(LOG_DEBUG, "Hotkey", "Registered hotkey id %d (modifiers: 0x%04X, vk: 0x%02X)", id, modifiers, vk);
    return TRUE;
}

void HotkeyUnregister(int id)
{
    UnregisterHotKey(NULL, id);
    LogMessage(LOG_DEBUG, "Hotkey", "Unregistered hotkey id %d", id);
}

void HotkeySetCallback(HotkeyCallback callback)
{
    hotkeyCallback = callback;
    LogMessage(LOG_DEBUG, "Hotkey", "Hotkey callback set");
}

BOOL HotkeyProcessMessage(MSG* msg)
{
    if (!msg) {
        return FALSE;
    }
    
    if (msg->message == WM_HOTKEY) {
        int hotkeyId = (int)msg->wParam;
        LogMessage(LOG_DEBUG, "Hotkey", "Hotkey pressed: id %d", hotkeyId);
        
        if (hotkeyCallback) {
            hotkeyCallback(hotkeyId);
        }
        return TRUE;
    }
    
    return FALSE;
}
