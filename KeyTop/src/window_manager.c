#include "window_manager.h"
#include "utils.h"
#include <windows.h>

BOOL WindowToggleTopmost(HWND hwnd) {
    if (!hwnd || !IsWindow(hwnd)) {
        LogMessage(LOG_ERROR, "window_manager", "Invalid window handle for topmost toggle");
        return FALSE;
    }

    BOOL isTopmost = WindowIsTopmost(hwnd);
    HWND insertAfter = isTopmost ? HWND_NOTOPMOST : HWND_TOPMOST;

    if (!SetWindowPos(hwnd, insertAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE)) {
        LogMessage(LOG_ERROR, "window_manager", "Failed to toggle topmost state");
        return FALSE;
    }

    LogMessage(LOG_INFO, "window_manager", "Window topmost state toggled: %s", isTopmost ? "OFF" : "ON");
    return TRUE;
}

BOOL WindowIsTopmost(HWND hwnd) {
    if (!hwnd || !IsWindow(hwnd)) {
        return FALSE;
    }

    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    return (exStyle & WS_EX_TOPMOST) != 0;
}

BOOL WindowToggleTransparent(HWND hwnd) {
    if (!hwnd || !IsWindow(hwnd)) {
        LogMessage(LOG_ERROR, "window_manager", "Invalid window handle for transparency toggle");
        return FALSE;
    }

    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    BOOL isTransparent = (exStyle & WS_EX_TRANSPARENT) != 0;

    if (isTransparent) {
        exStyle &= ~WS_EX_TRANSPARENT;
    } else {
        exStyle |= WS_EX_TRANSPARENT;
    }

    if (!SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle)) {
        LogMessage(LOG_ERROR, "window_manager", "Failed to toggle transparency state");
        return FALSE;
    }

    LogMessage(LOG_INFO, "window_manager", "Window transparency state toggled: %s", isTransparent ? "OFF" : "ON");
    return TRUE;
}

BOOL WindowIsTransparent(HWND hwnd) {
    if (!hwnd || !IsWindow(hwnd)) {
        return FALSE;
    }

    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    return (exStyle & WS_EX_TRANSPARENT) != 0;
}

HWND WindowGetForegroundWindow(void) {
    return GetForegroundWindow();
}

int WindowEnumWindows(WINDOWPROC callback, LPARAM lParam) {
    if (!callback) {
        LogMessage(LOG_ERROR, "window_manager", "Invalid callback for window enumeration");
        return 0;
    }

    int count = 0;
    HWND hwnd = GetWindow(GW_HWNDNEXT);
    
    while (hwnd != NULL) {
        if (IsWindowVisible(hwnd)) {
            callback(hwnd, lParam);
            count++;
        }
        hwnd = GetWindow(hwnd, GW_HWNDNEXT);
    }

    return count;
}

BOOL WindowGetInfo(HWND hwnd, WindowInfo* info) {
    if (!hwnd || !info || !IsWindow(hwnd)) {
        LogMessage(LOG_ERROR, "window_manager", "Invalid parameters for WindowGetInfo");
        return FALSE;
    }

    ZeroMemory(info, sizeof(WindowInfo));

    info->hwnd = hwnd;

    if (!GetWindowText(hwnd, info->title, MAX_PATH_LENGTH)) {
        info->title[0] = _T('\0');
    }

    if (!GetClassName(hwnd, info->className, MAX_PATH_LENGTH)) {
        info->className[0] = _T('\0');
    }

    if (!GetWindowRect(hwnd, &info->rect)) {
        ZeroMemory(&info->rect, sizeof(RECT));
    }

    info->isTopmost = WindowIsTopmost(hwnd);
    info->isTransparent = WindowIsTransparent(hwnd);

    return TRUE;
}

BOOL WindowSetTransparency(HWND hwnd, BYTE alpha) {
    if (!hwnd || !IsWindow(hwnd)) {
        LogMessage(LOG_ERROR, "window_manager", "Invalid window handle for transparency setting");
        return FALSE;
    }

    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    exStyle |= WS_EX_LAYERED;
    
    if (!SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle)) {
        LogMessage(LOG_ERROR, "window_manager", "Failed to set WS_EX_LAYERED style");
        return FALSE;
    }

    if (!SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA)) {
        LogMessage(LOG_ERROR, "window_manager", "Failed to set layered window attributes");
        return FALSE;
    }

    LogMessage(LOG_INFO, "window_manager", "Window transparency set to %d", alpha);
    return TRUE;
}
