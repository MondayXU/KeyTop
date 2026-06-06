#include "mouse_hook.h"
#include "utils.h"
#include <windows.h>

// Static variables
static HHOOK mouseHook = NULL;
static AppConfig* currentConfig = NULL;
static MouseCallback mouseCallback = NULL;
static BOOL enabled = FALSE;

// Helper function to check if click is on title bar
static BOOL IsClickOnTitleBar(int x, int y) {
    POINT clickPoint;
    clickPoint.x = x;
    clickPoint.y = y;

    // Get the window at the click point
    HWND hwnd = WindowFromPoint(clickPoint);
    if (hwnd == NULL) {
        return FALSE;
    }

    RECT windowRect, clientRect;
    if (!GetWindowRect(hwnd, &windowRect)) {
        return FALSE;
    }

    if (!GetClientRect(hwnd, &clientRect)) {
        return FALSE;
    }

    // Convert client rect to screen coordinates for comparison
    POINT clientTopLeft = { clientRect.left, clientRect.top };
    ClientToScreen(hwnd, &clientTopLeft);

    // Calculate title bar height
    int titleBarHeight = clientTopLeft.y - windowRect.top;

    // Check if click point is within title bar area
    // Title bar is between windowRect.top and clientTopLeft.y
    // and horizontally within the window
    if (y >= windowRect.top && y < clientTopLeft.y &&
        x >= windowRect.left && x <= windowRect.right) {
        return TRUE;
    }

    return FALSE;
}

// Low-level mouse hook callback
static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && enabled && currentConfig != NULL && currentConfig->enableMidClick) {
        if (wParam == WM_MBUTTONDOWN || wParam == WM_MBUTTONUP) {
            MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;
            if (pMouseStruct != NULL) {
                int x = pMouseStruct->pt.x;
                int y = pMouseStruct->pt.y;

                if (IsClickOnTitleBar(x, y)) {
                    if (mouseCallback != NULL) {
                        mouseCallback(x, y, (UINT)wParam);
                    }
                }
            }
        }
    }

    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

// Initialize the mouse hook
BOOL MouseHookInit(AppConfig* config) {
    if (config == NULL) {
        return FALSE;
    }

    // If already initialized, shutdown first
    if (mouseHook != NULL) {
        MouseHookShutdown();
    }

    currentConfig = config;
    enabled = TRUE;

    // Install the low-level mouse hook
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, GetModuleHandle(NULL), 0);
    if (mouseHook == NULL) {
        LogMessage(LOG_ERROR, "MouseHook", "Failed to install mouse hook: %lu", GetLastError());
        enabled = FALSE;
        currentConfig = NULL;
        return FALSE;
    }

    return TRUE;
}

// Shutdown the mouse hook
void MouseHookShutdown(void) {
    if (mouseHook != NULL) {
        UnhookWindowsHookEx(mouseHook);
        mouseHook = NULL;
    }
    enabled = FALSE;
    currentConfig = NULL;
    mouseCallback = NULL;
}

// Reload configuration
void MouseHookReloadConfig(AppConfig* config) {
    if (config != NULL) {
        currentConfig = config;
    }
}

// Set the mouse callback function
void MouseHookSetCallback(MouseCallback callback) {
    mouseCallback = callback;
}

// Check if mouse hook is enabled
BOOL MouseHookIsEnabled(void) {
    return enabled;
}