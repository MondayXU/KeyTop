#ifndef TRAY_ICON_H
#define TRAY_ICON_H

#include "keytop.h"

#define TRAY_ICON_ID 1001
#define WM_TRAYICON (WM_USER + 1)

typedef void (*TrayMenuCallback)(int menuId);

BOOL TrayIconCreate(HINSTANCE hInstance, HWND hwnd);
void TrayIconDestroy(HWND hwnd);
void TrayIconShowBalloon(HWND hwnd, const char* title, const char* message);
void TrayIconSetIcon(HWND hwnd, HICON hIcon);
void TrayIconSetTooltip(HWND hwnd, const char* tooltip);
void TrayIconSetMenuCallback(TrayMenuCallback callback);
void TrayIconShowMenu(HWND hwnd);
HICON TrayIconLoadIcon(HINSTANCE hInstance, int iconId);

#endif /* TRAY_ICON_H */