#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "keytop.h"

typedef struct {
    HWND hwnd;
    TCHAR title[MAX_PATH_LENGTH];
    TCHAR className[MAX_PATH_LENGTH];
    RECT rect;
    BOOL isTopmost;
    BOOL isTransparent;
} WindowInfo;

BOOL WindowToggleTopmost(HWND hwnd);
BOOL WindowIsTopmost(HWND hwnd);
BOOL WindowToggleTransparent(HWND hwnd);
BOOL WindowIsTransparent(HWND hwnd);
HWND WindowGetForegroundWindow(void);
int WindowEnumWindows(WINDOWPROC callback, LPARAM lParam);
BOOL WindowGetInfo(HWND hwnd, WindowInfo* info);
BOOL WindowSetTransparency(HWND hwnd, BYTE alpha);

#endif /* WINDOW_MANAGER_H */
