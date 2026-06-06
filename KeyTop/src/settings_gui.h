#ifndef SETTINGS_GUI_H
#define SETTINGS_GUI_H

#include "keytop.h"
#include "config.h"

#define SETTINGS_TAB_KEYBOARD 0
#define SETTINGS_TAB_WINDOW 1
#define SETTINGS_TAB_HOTKEYS 2

BOOL SettingsCreate(HINSTANCE hInstance, HWND parent, AppConfig* config);
void SettingsDestroy(void);
void SettingsShow(HWND parent);
void SettingsHide(void);
void SettingsApply(void);
BOOL SettingsIsVisible(void);

#endif /* SETTINGS_GUI_H */