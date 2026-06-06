#ifndef HOTKEY_H
#define HOTKEY_H

#include "keytop.h"
#include "config.h"

#define HOTKEY_TOPMOST 1001
#define HOTKEY_TRANSPARENT 1002

typedef void (*HotkeyCallback)(int id);

BOOL HotkeyInit(HINSTANCE hInstance);
void HotkeyShutdown(void);
BOOL HotkeyRegister(int id, UINT modifiers, UINT vk);
void HotkeyUnregister(int id);
void HotkeySetCallback(HotkeyCallback callback);
BOOL HotkeyProcessMessage(MSG* msg);

#endif /* HOTKEY_H */
