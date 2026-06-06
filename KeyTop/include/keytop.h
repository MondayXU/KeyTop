#ifndef KEYTOP_H
#define KEYTOP_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#define APP_NAME _T("KeyTop")
#define APP_VERSION _T("1.0.0")
#define CONFIG_FILE _T("KeyTop.ini")
#define LOG_FILE _T("KeyTop.log")

#define MAX_PATH_LENGTH 256
#define MAX_KEY_NAME_LENGTH 64
#define MAX_MAPPING_COUNT 100

#define WM_TRAYICON (WM_USER + 1)
#define WM_RELOAD_CONFIG (WM_USER + 2)

#endif /* KEYTOP_H */
