#ifndef MOUSE_HOOK_H
#define MOUSE_HOOK_H

#include "keytop.h"
#include "config.h"

typedef void (*MouseCallback)(int x, int y, UINT message);

BOOL MouseHookInit(AppConfig* config);
void MouseHookShutdown(void);
void MouseHookReloadConfig(AppConfig* config);
void MouseHookSetCallback(MouseCallback callback);
BOOL MouseHookIsEnabled(void);

#endif /* MOUSE_HOOK_H */