#include "tray_icon.h"
#include <shellapi.h>
#include <string.h>

// Menu IDs
#define MENU_SETTINGS 2001
#define MENU_ENABLE_KEYBOARD_HOOK 2002
#define MENU_ENABLE_MOUSE_HOOK 2003
#define MENU_ABOUT 2004
#define MENU_EXIT 2005

// Static variables
static TrayMenuCallback menuCallback = NULL;
static HICON hTrayIcon = NULL;
static NOTIFYICONDATA nid;

// Internal helper functions
static BOOL TrayIconAdd(HWND hwnd) {
    memset(&nid, 0, sizeof(NOTIFYICONDATA));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = TRAY_ICON_ID;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = hTrayIcon;
    _tcsncpy_s(nid.szTip, _countof(nid.szTip), _T("KeyTop"), _TRUNCATE);

    return Shell_NotifyIcon(NIM_ADD, &nid);
}

static BOOL TrayIconRemove(void) {
    return Shell_NotifyIcon(NIM_DELETE, &nid);
}

// Public API implementation
BOOL TrayIconCreate(HINSTANCE hInstance, HWND hwnd) {
    if (hInstance == NULL || hwnd == NULL) {
        return FALSE;
    }

    // Load icon
    hTrayIcon = TrayIconLoadIcon(hInstance, 1);
    if (hTrayIcon == NULL) {
        return FALSE;
    }

    return TrayIconAdd(hwnd);
}

void TrayIconDestroy(HWND hwnd) {
    if (hwnd != NULL) {
        TrayIconRemove();
    }

    if (hTrayIcon != NULL) {
        DestroyIcon(hTrayIcon);
        hTrayIcon = NULL;
    }
}

void TrayIconShowBalloon(HWND hwnd, const char* title, const char* message) {
    if (hwnd == NULL || title == NULL || message == NULL) {
        return;
    }

    nid.uFlags |= NIF_INFO;
    _tcsncpy_s(nid.szInfoTitle, _countof(nid.szInfoTitle), title, _TRUNCATE);
    _tcsncpy_s(nid.szInfo, _countof(nid.szInfo), message, _TRUNCATE);
    nid.dwInfoFlags = NIIF_INFO;

    Shell_NotifyIcon(NIM_MODIFY, &nid);

    // Clear info flags after showing
    nid.uFlags &= ~NIF_INFO;
    memset(nid.szInfoTitle, 0, sizeof(nid.szInfoTitle));
    memset(nid.szInfo, 0, sizeof(nid.szInfo));
}

void TrayIconSetIcon(HWND hwnd, HICON hIcon) {
    if (hwnd == NULL || hIcon == NULL) {
        return;
    }

    if (hTrayIcon != NULL) {
        DestroyIcon(hTrayIcon);
    }

    hTrayIcon = hIcon;
    nid.hIcon = hIcon;

    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

void TrayIconSetTooltip(HWND hwnd, const char* tooltip) {
    if (hwnd == NULL || tooltip == NULL) {
        return;
    }

    _tcsncpy_s(nid.szTip, _countof(nid.szTip), tooltip, _TRUNCATE);
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

void TrayIconSetMenuCallback(TrayMenuCallback callback) {
    menuCallback = callback;
}

void TrayIconShowMenu(HWND hwnd) {
    if (hwnd == NULL) {
        return;
    }

    HMENU hMenu = CreatePopupMenu();
    if (hMenu == NULL) {
        return;
    }

    AppendMenu(hMenu, MF_STRING, MENU_SETTINGS, _T("Settings"));
    AppendMenu(hMenu, MF_STRING, MENU_ENABLE_KEYBOARD_HOOK, _T("Enable Keyboard Hook"));
    AppendMenu(hMenu, MF_STRING, MENU_ENABLE_MOUSE_HOOK, _T("Enable Mouse Hook"));
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenu, MF_STRING, MENU_ABOUT, _T("About"));
    AppendMenu(hMenu, MF_STRING, MENU_EXIT, _T("Exit"));

    POINT pt;
    GetCursorPos(&pt);

    SetForegroundWindow(hwnd);
    int menuId = TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwnd, NULL);

    if (menuId > 0 && menuCallback != NULL) {
        menuCallback(menuId);
    }

    DestroyMenu(hMenu);
}

HICON TrayIconLoadIcon(HINSTANCE hInstance, int iconId) {
    if (hInstance == NULL) {
        return NULL;
    }

    HICON hIcon = (HICON)LoadImage(hInstance,
                                    MAKEINTRESOURCE(iconId),
                                    IMAGE_ICON,
                                    GetSystemMetrics(SM_CXICON),
                                    GetSystemMetrics(SM_CYICON),
                                    LR_DEFAULTCOLOR);

    return hIcon;
}