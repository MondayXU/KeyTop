#include "keytop.h"
#include "utils.h"
#include "config.h"
#include "window_manager.h"
#include "keyboard_hook.h"
#include "mouse_hook.h"
#include "hotkey.h"
#include "tray_icon.h"
#include "settings_gui.h"
#include "error_handler.h"

static AppConfig appConfig;
static HINSTANCE hInstance;
static HWND hMainWnd;
static BOOL keyboardHookEnabled = FALSE;
static BOOL mouseHookEnabled = FALSE;

static void OnHotkey(int id)
{
    switch (id) {
        case HOTKEY_TOPMOST:
            HWND hwnd = WindowGetForegroundWindow();
            if (hwnd) {
                WindowToggleTopmost(hwnd);
                if (appConfig.showNotifications) {
                    TrayIconShowBalloon(hMainWnd, "Window Topmost", "Toggled");
                }
            }
            break;

        case HOTKEY_TRANSPARENT:
            hwnd = WindowGetForegroundWindow();
            if (hwnd) {
                WindowToggleTransparent(hwnd);
                if (appConfig.showNotifications) {
                    TrayIconShowBalloon(hMainWnd, "Window Transparent", "Toggled");
                }
            }
            break;
    }
}

static void OnMouseClick(int x, int y, UINT message)
{
    if (message == WM_MBUTTONDOWN) {
        POINT pt = {x, y};
        HWND hwnd = WindowFromPoint(pt);
        if (hwnd) {
            WindowToggleTopmost(hwnd);
            if (appConfig.showNotifications) {
                TrayIconShowBalloon(hMainWnd, "Window Topmost", "Toggled");
            }
        }
    }
}

static void OnMenu(int menuId)
{
    switch (menuId) {
        case 2001:
            SettingsShow(hMainWnd);
            break;

        case 2002:
            keyboardHookEnabled = !keyboardHookEnabled;
            if (keyboardHookEnabled) {
                KeyboardHookInit(&appConfig);
            } else {
                KeyboardHookShutdown();
            }
            break;

        case 2003:
            mouseHookEnabled = !mouseHookEnabled;
            if (mouseHookEnabled) {
                MouseHookInit(&appConfig);
            } else {
                MouseHookShutdown();
            }
            break;

        case 2004:
            MessageBox(hMainWnd, "KeyTop v1.0.0\n\nLightweight keyboard remapping and window management tool.",
                       "About KeyTop", MB_OK | MB_ICONINFORMATION);
            break;

        case 2005:
            PostQuitMessage(0);
            break;
    }
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_CREATE:
            hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

            LogSetLevel(appConfig.logLevel);
            LogSetFile(GetLogPath());

            TrayIconCreate(hInstance, hwnd);
            TrayIconSetMenuCallback(OnMenu);

            HotkeyInit(hInstance);
            HotkeySetCallback(OnHotkey);
            HotkeyRegister(HOTKEY_TOPMOST, appConfig.topkey.modifiers, appConfig.topkey.vk);
            HotkeyRegister(HOTKEY_TRANSPARENT, appConfig.transkey.modifiers, appConfig.transkey.vk);

            KeyboardHookInit(&appConfig);
            keyboardHookEnabled = TRUE;

            if (appConfig.enableMidClick) {
                MouseHookInit(&appConfig);
                mouseHookEnabled = TRUE;
            }
            return 0;

        case WM_TRAYICON:
            if (lParam == WM_RBUTTONUP || lParam == WM_LBUTTONDBLCLK) {
                TrayIconShowMenu(hwnd);
            }
            return 0;

        case WM_RELOAD_CONFIG:
            ConfigLoad(&appConfig, GetConfigPath());
            KeyboardHookReloadConfig(&appConfig);
            MouseHookReloadConfig(&appConfig);
            return 0;

        case WM_DESTROY:
            KeyboardHookShutdown();
            MouseHookShutdown();
            HotkeyShutdown();
            TrayIconDestroy(hwnd);
            PostQuitMessage(0);
            return 0;

        default:
            HotkeyProcessMessage((MSG*)&lParam);
            break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    hInstance = hInst;
    hPrevInstance;
    lpCmdLine;
    nCmdShow;

    ConfigGetDefaults(&appConfig);
    ConfigLoad(&appConfig, GetConfigPath());

    LogSetLevel(appConfig.logLevel);
    LogSetFile(GetLogPath());
    LogMessage(LOG_INFO, "Main", "KeyTop starting...");

    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "KeyTopClass";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        ErrorShowDialog("Error", "Failed to register window class");
        return 1;
    }

    hMainWnd = CreateWindowEx(0, "KeyTopClass", APP_NAME, WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
                              NULL, NULL, hInstance, NULL);

    if (!hMainWnd) {
        ErrorShowDialog("Error", "Failed to create window");
        return 1;
    }

    ShowWindow(hMainWnd, SW_HIDE);
    UpdateWindow(hMainWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    LogMessage(LOG_INFO, "Main", "KeyTop exiting...");
    return (int)msg.wParam;
}
