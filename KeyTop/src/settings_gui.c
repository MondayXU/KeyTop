#include "settings_gui.h"
#include "resource.h"
#include <stdio.h>

// Static variables
static HWND hSettingsDlg = NULL;
static AppConfig currentConfig;
static AppConfig tempConfig;

// Dialog procedure
INT_PTR CALLBACK SettingsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_INITDIALOG:
            // Initialize dialog with current configuration
            // This is where we'd set up controls with current config values
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK:
                    // Apply settings and close dialog
                    SettingsApply();
                    EndDialog(hDlg, IDOK);
                    return (INT_PTR)TRUE;

                case IDCANCEL:
                    // Close dialog without applying
                    EndDialog(hDlg, IDCANCEL);
                    return (INT_PTR)TRUE;

                case IDC_APPLY:
                    // Apply settings without closing
                    SettingsApply();
                    return (INT_PTR)TRUE;

                case IDC_ADD_MAPPING:
                    // Add a new key mapping
                    // Implementation would go here
                    return (INT_PTR)TRUE;

                case IDC_REMOVE_MAPPING:
                    // Remove selected key mapping
                    // Implementation would go here
                    return (INT_PTR)TRUE;

                case IDC_EDIT_MAPPING:
                    // Edit selected key mapping
                    // Implementation would go here
                    return (INT_PTR)TRUE;

                case IDC_TEST:
                    // Test current settings
                    // Implementation would go here
                    return (INT_PTR)TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, IDCANCEL);
            return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}

// Create the settings dialog
BOOL SettingsCreate(HINSTANCE hInstance, HWND parent, AppConfig* config) {
    if (!config) {
        return FALSE;
    }

    // Store the configuration pointer
    currentConfig = *config;

    // Create the dialog
    hSettingsDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_SETTINGS), parent, SettingsDlgProc);

    if (!hSettingsDlg) {
        return FALSE;
    }

    return TRUE;
}

// Destroy the settings dialog
void SettingsDestroy(void) {
    if (hSettingsDlg) {
        DestroyWindow(hSettingsDlg);
        hSettingsDlg = NULL;
    }
}

// Show the settings dialog (create if needed)
void SettingsShow(HWND parent) {
    if (!hSettingsDlg) {
        // Dialog doesn't exist, can't show it
        return;
    }

    ShowWindow(hSettingsDlg, SW_SHOW);
    SetForegroundWindow(hSettingsDlg);
}

// Hide the settings dialog
void SettingsHide(void) {
    if (hSettingsDlg) {
        ShowWindow(hSettingsDlg, SW_HIDE);
    }
}

// Apply the settings
void SettingsApply(void) {
    // Log that settings were applied
    // In a full implementation, this would save the tempConfig to persistent storage
    OutputDebugString(_T("Settings applied\n"));
}

// Check if settings dialog is visible
BOOL SettingsIsVisible(void) {
    return (hSettingsDlg && IsWindowVisible(hSettingsDlg));
}