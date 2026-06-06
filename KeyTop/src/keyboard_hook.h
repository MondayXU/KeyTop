#ifndef KEYBOARD_HOOK_H
#define KEYBOARD_HOOK_H

#include "keytop.h"
#include "config.h"

typedef void (*KeyCallback)(UINT vk, BOOL keyDown);

BOOL KeyboardHookInit(AppConfig* config);
void KeyboardHookShutdown(void);
void KeyboardHookReloadConfig(AppConfig* config);
void KeyboardHookSetCallback(KeyCallback callback);
BOOL KeyboardHookIsEnabled(void);

#endif /* KEYBOARD_HOOK_H */
