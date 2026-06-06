#include "config.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Internal function declarations
static const char* GetKeyName(UINT vk);
static UINT GetVkFromName(const char* name);
static void ParseHotkey(const char* hotkeyStr, UINT* modifiers, UINT* vk);
static UINT ParseModifiers(const char* modStr);
static BOOL WritePrivateProfileString(const char* section, const char* key, const char* value, const char* filename);
static DWORD GetPrivateProfileString(const char* section, const char* key, const char* defaultValue, char* buffer, DWORD bufferSize, const char* filename);
static int GetPrivateProfileInt(const char* section, const char* key, int defaultValue, const char* filename);
static BOOL WritePrivateProfileInt(const char* section, const char* key, int value, const char* filename);
static char* TrimWhitespace(char* str);

// Get key name from VK code
const char* GetKeyName(UINT vk) {
    static char buffer[MAX_KEY_NAME_LENGTH];

    // Handle special keys
    switch (vk) {
        case VK_BACK: return "Backspace";
        case VK_TAB: return "Tab";
        case VK_RETURN: return "Enter";
        case VK_PAUSE: return "Pause";
        case VK_CAPITAL: return "CapsLock";
        case VK_ESCAPE: return "Escape";
        case VK_SPACE: return "Space";
        case VK_PRIOR: return "PageUp";
        case VK_NEXT: return "PageDown";
        case VK_END: return "End";
        case VK_HOME: return "Home";
        case VK_LEFT: return "Left";
        case VK_UP: return "Up";
        case VK_RIGHT: return "Right";
        case VK_DOWN: return "Down";
        case VK_SNAPSHOT: return "PrintScreen";
        case VK_INSERT: return "Insert";
        case VK_DELETE: return "Delete";
        case VK_NUMPAD0: return "Num0";
        case VK_NUMPAD1: return "Num1";
        case VK_NUMPAD2: return "Num2";
        case VK_NUMPAD3: return "Num3";
        case VK_NUMPAD4: return "Num4";
        case VK_NUMPAD5: return "Num5";
        case VK_NUMPAD6: return "Num6";
        case VK_NUMPAD7: return "Num7";
        case VK_NUMPAD8: return "Num8";
        case VK_NUMPAD9: return "Num9";
        case VK_MULTIPLY: return "Num*";
        case VK_ADD: return "Num+";
        case VK_SUBTRACT: return "Num-";
        case VK_DECIMAL: return "Num.";
        case VK_DIVIDE: return "Num/";
        case VK_F1: return "F1";
        case VK_F2: return "F2";
        case VK_F3: return "F3";
        case VK_F4: return "F4";
        case VK_F5: return "F5";
        case VK_F6: return "F6";
        case VK_F7: return "F7";
        case VK_F8: return "F8";
        case VK_F9: return "F9";
        case VK_F10: return "F10";
        case VK_F11: return "F11";
        case VK_F12: return "F12";
        case VK_NUMLOCK: return "NumLock";
        case VK_SCROLL: return "ScrollLock";
        case VK_LSHIFT: return "LShift";
        case VK_RSHIFT: return "RShift";
        case VK_LCONTROL: return "LCtrl";
        case VK_RCONTROL: return "RCtrl";
        case VK_LMENU: return "LAlt";
        case VK_RMENU: return "RAlt";
        default:
            // For standard ASCII keys
            if (vk >= 'A' && vk <= 'Z') {
                snprintf(buffer, sizeof(buffer), "%c", (char)vk);
                return buffer;
            }
            if (vk >= '0' && vk <= '9') {
                snprintf(buffer, sizeof(buffer), "%c", (char)vk);
                return buffer;
            }
            // Try to get the key name using Windows API
            LONG lParam = MapVirtualKey(vk, MAPVK_VK_TO_VSC) << 16;
            if (GetKeyNameText(lParam, buffer, sizeof(buffer)) > 0) {
                return buffer;
            }
            snprintf(buffer, sizeof(buffer), "VK_%02X", vk);
            return buffer;
    }
}

// Get VK code from key name
UINT GetVkFromName(const char* name) {
    if (!name || !name[0]) return 0;

    // Handle special keys
    if (stricmp(name, "Backspace") == 0) return VK_BACK;
    if (stricmp(name, "Tab") == 0) return VK_TAB;
    if (stricmp(name, "Enter") == 0) return VK_RETURN;
    if (stricmp(name, "Pause") == 0) return VK_PAUSE;
    if (stricmp(name, "CapsLock") == 0) return VK_CAPITAL;
    if (stricmp(name, "Escape") == 0) return VK_ESCAPE;
    if (stricmp(name, "Space") == 0) return VK_SPACE;
    if (stricmp(name, "PageUp") == 0) return VK_PRIOR;
    if (stricmp(name, "PageDown") == 0) return VK_NEXT;
    if (stricmp(name, "End") == 0) return VK_END;
    if (stricmp(name, "Home") == 0) return VK_HOME;
    if (stricmp(name, "Left") == 0) return VK_LEFT;
    if (stricmp(name, "Up") == 0) return VK_UP;
    if (stricmp(name, "Right") == 0) return VK_RIGHT;
    if (stricmp(name, "Down") == 0) return VK_DOWN;
    if (stricmp(name, "PrintScreen") == 0) return VK_SNAPSHOT;
    if (stricmp(name, "Insert") == 0) return VK_INSERT;
    if (stricmp(name, "Delete") == 0) return VK_DELETE;
    if (stricmp(name, "Num0") == 0) return VK_NUMPAD0;
    if (stricmp(name, "Num1") == 0) return VK_NUMPAD1;
    if (stricmp(name, "Num2") == 0) return VK_NUMPAD2;
    if (stricmp(name, "Num3") == 0) return VK_NUMPAD3;
    if (stricmp(name, "Num4") == 0) return VK_NUMPAD4;
    if (stricmp(name, "Num5") == 0) return VK_NUMPAD5;
    if (stricmp(name, "Num6") == 0) return VK_NUMPAD6;
    if (stricmp(name, "Num7") == 0) return VK_NUMPAD7;
    if (stricmp(name, "Num8") == 0) return VK_NUMPAD8;
    if (stricmp(name, "Num9") == 0) return VK_NUMPAD9;
    if (stricmp(name, "Num*") == 0) return VK_MULTIPLY;
    if (stricmp(name, "Num+") == 0) return VK_ADD;
    if (stricmp(name, "Num-") == 0) return VK_SUBTRACT;
    if (stricmp(name, "Num.") == 0) return VK_DECIMAL;
    if (stricmp(name, "Num/") == 0) return VK_DIVIDE;
    if (stricmp(name, "F1") == 0) return VK_F1;
    if (stricmp(name, "F2") == 0) return VK_F2;
    if (stricmp(name, "F3") == 0) return VK_F3;
    if (stricmp(name, "F4") == 0) return VK_F4;
    if (stricmp(name, "F5") == 0) return VK_F5;
    if (stricmp(name, "F6") == 0) return VK_F6;
    if (stricmp(name, "F7") == 0) return VK_F7;
    if (stricmp(name, "F8") == 0) return VK_F8;
    if (stricmp(name, "F9") == 0) return VK_F9;
    if (stricmp(name, "F10") == 0) return VK_F10;
    if (stricmp(name, "F11") == 0) return VK_F11;
    if (stricmp(name, "F12") == 0) return VK_F12;
    if (stricmp(name, "NumLock") == 0) return VK_NUMLOCK;
    if (stricmp(name, "ScrollLock") == 0) return VK_SCROLL;
    if (stricmp(name, "LShift") == 0) return VK_LSHIFT;
    if (stricmp(name, "RShift") == 0) return VK_RSHIFT;
    if (stricmp(name, "LCtrl") == 0) return VK_LCONTROL;
    if (stricmp(name, "RCtrl") == 0) return VK_RCONTROL;
    if (stricmp(name, "LAlt") == 0) return VK_LMENU;
    if (stricmp(name, "RAlt") == 0) return VK_RMENU;

    // For single character keys
    if (strlen(name) == 1) {
        char c = name[0];
        if (c >= 'A' && c <= 'Z') return c;
        if (c >= 'a' && c <= 'z') return toupper(c);
        if (c >= '0' && c <= '9') return c;
    }

    // Try to find by display name using Windows API
    // This is a simplified approach - in production you might want to enumerate all VKs
    for (UINT vk = 0; vk < 256; vk++) {
        const char* keyName = GetKeyName(vk);
        if (stricmp(name, keyName) == 0) {
            return vk;
        }
    }

    return 0;
}

// Parse modifier string (e.g., "Ctrl+Shift")
UINT ParseModifiers(const char* modStr) {
    UINT modifiers = 0;
    char buffer[256];
    strncpy(buffer, modStr, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char* token = strtok(buffer, "+");
    while (token) {
        token = TrimWhitespace(token);
        if (stricmp(token, "Ctrl") == 0) {
            modifiers |= MOD_CTRL;
        } else if (stricmp(token, "Alt") == 0) {
            modifiers |= MOD_ALT;
        } else if (stricmp(token, "Shift") == 0) {
            modifiers |= MOD_SHIFT;
        } else if (stricmp(token, "Win") == 0) {
            modifiers |= MOD_WIN;
        }
        token = strtok(NULL, "+");
    }

    return modifiers;
}

// Parse hotkey string (e.g., "Ctrl+Space")
void ParseHotkey(const char* hotkeyStr, UINT* modifiers, UINT* vk) {
    *modifiers = 0;
    *vk = 0;

    if (!hotkeyStr || !hotkeyStr[0]) return;

    char buffer[256];
    strncpy(buffer, hotkeyStr, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // Find the last '+' to separate modifiers from key
    char* lastPlus = strrchr(buffer, '+');
    if (lastPlus) {
        *lastPlus = '\0';
        char* keyPart = lastPlus + 1;
        *modifiers = ParseModifiers(buffer);
        *vk = GetVkFromName(TrimWhitespace(keyPart));
    } else {
        // No modifiers, just a key
        *vk = GetVkFromName(TrimWhitespace(buffer));
    }
}

// Trim whitespace from string
char* TrimWhitespace(char* str) {
    if (!str) return str;

    // Trim leading whitespace
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return str;

    // Trim trailing whitespace
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';

    return str;
}

// Custom INI file functions (since we're using pure Win32 API without MFC)
BOOL WritePrivateProfileString(const char* section, const char* key, const char* value, const char* filename) {
    if (!section || !key || !value || !filename) return FALSE;

    FILE* file = fopen(filename, "r");
    char tempFilename[MAX_PATH_LENGTH];
    snprintf(tempFilename, sizeof(tempFilename), "%s.tmp", filename);

    FILE* tempFile = fopen(tempFilename, "w");
    if (!tempFile) {
        if (file) fclose(file);
        return FALSE;
    }

    char line[1024];
    char currentSection[MAX_PATH_LENGTH] = "";
    BOOL sectionFound = FALSE;
    BOOL keyWritten = FALSE;

    if (file) {
        while (fgets(line, sizeof(line), file)) {
            // Remove newline
            size_t len = strlen(line);
            while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
                line[--len] = '\0';
            }

            // Check for section header
            if (line[0] == '[' && line[len-1] == ']') {
                // If we were in the target section and haven't written the key yet
                if (sectionFound && !keyWritten) {
                    fprintf(tempFile, "%s=%s\n", key, value);
                    keyWritten = TRUE;
                }

                strncpy(currentSection, line + 1, len - 2);
                currentSection[len - 2] = '\0';
                fprintf(tempFile, "%s\n", line);

                if (stricmp(currentSection, section) == 0) {
                    sectionFound = TRUE;
                }
            } else if (sectionFound && !keyWritten) {
                // Check if this line has the key we want to write
                char* equals = strchr(line, '=');
                if (equals) {
                    *equals = '\0';
                    char* lineKey = TrimWhitespace(line);
                    if (stricmp(lineKey, key) == 0) {
                        fprintf(tempFile, "%s=%s\n", key, value);
                        keyWritten = TRUE;
                    } else {
                        fprintf(tempFile, "%s=%s\n", lineKey, equals + 1);
                    }
                    *equals = '='; // Restore for potential future use
                } else {
                    fprintf(tempFile, "%s\n", line);
                }
            } else {
                fprintf(tempFile, "%s\n", line);
            }
        }
        fclose(file);
    }

    // If section wasn't found, add it
    if (!sectionFound) {
        fprintf(tempFile, "[%s]\n", section);
        fprintf(tempFile, "%s=%s\n", key, value);
    } else if (!keyWritten) {
        fprintf(tempFile, "%s=%s\n", key, value);
    }

    fclose(tempFile);

    // Replace original file with temp file
    remove(filename);
    if (rename(tempFilename, filename) != 0) {
        return FALSE;
    }

    return TRUE;
}

DWORD GetPrivateProfileString(const char* section, const char* key, const char* defaultValue,
                             char* buffer, DWORD bufferSize, const char* filename) {
    if (!section || !key || !buffer || !filename || bufferSize == 0) return 0;

    strncpy(buffer, defaultValue ? defaultValue : "", bufferSize - 1);
    buffer[bufferSize - 1] = '\0';

    FILE* file = fopen(filename, "r");
    if (!file) return strlen(buffer);

    char line[1024];
    char currentSection[MAX_PATH_LENGTH] = "";
    BOOL inTargetSection = FALSE;

    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[--len] = '\0';
        }

        // Skip empty lines and comments
        if (len == 0 || line[0] == ';' || line[0] == '#') continue;

        // Check for section header
        if (line[0] == '[' && line[len-1] == ']') {
            strncpy(currentSection, line + 1, len - 2);
            currentSection[len - 2] = '\0';
            inTargetSection = (stricmp(currentSection, section) == 0);
            continue;
        }

        // If we're in the target section, look for the key
        if (inTargetSection) {
            char* equals = strchr(line, '=');
            if (equals) {
                *equals = '\0';
                char* lineKey = TrimWhitespace(line);
                if (stricmp(lineKey, key) == 0) {
                    strncpy(buffer, equals + 1, bufferSize - 1);
                    buffer[bufferSize - 1] = '\0';
                    fclose(file);
                    return strlen(buffer);
                }
                *equals = '=';
            }
        }
    }

    fclose(file);
    return strlen(buffer);
}

int GetPrivateProfileInt(const char* section, const char* key, int defaultValue, const char* filename) {
    char buffer[64];
    GetPrivateProfileString(section, key, "", buffer, sizeof(buffer), filename);
    if (strlen(buffer) == 0) return defaultValue;
    return atoi(buffer);
}

BOOL WritePrivateProfileInt(const char* section, const char* key, int value, const char* filename) {
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%d", value);
    return WritePrivateProfileString(section, key, buffer, filename);
}

// Get default configuration values
void ConfigGetDefaults(AppConfig* config) {
    if (!config) return;

    memset(config, 0, sizeof(AppConfig));

    config->autoStart = 0;
    config->logLevel = LOG_INFO;
    config->showNotifications = 1;

    // Default topkey: Ctrl+Space
    config->topkey.modifiers = MOD_CTRL;
    config->topkey.vk = VK_SPACE;

    // Default transkey: Ctrl+Shift+Space
    config->transkey.modifiers = MOD_CTRL | MOD_SHIFT;
    config->transkey.vk = VK_SPACE;

    config->enableMidClick = 1;

    // Default mapping: CapsLock -> Escape
    ConfigAddMapping(config, "CapsLock", "Escape");
}

// Load configuration from INI file
BOOL ConfigLoad(AppConfig* config, const char* filename) {
    if (!config || !filename) return FALSE;

    // Start with defaults
    ConfigGetDefaults(config);

    char buffer[256];
    char modBuffer[128];

    // Load general settings
    config->autoStart = GetPrivateProfileInt("General", "AutoStart", 0, filename);
    config->logLevel = GetPrivateProfileInt("General", "LogLevel", LOG_INFO, filename);
    config->showNotifications = GetPrivateProfileInt("General", "ShowNotifications", 1, filename);

    // Load window settings
    config->enableMidClick = GetPrivateProfileInt("Window", "EnableMidClick", 1, filename);

    // Load topkey
    GetPrivateProfileString("Window", "TopkeyMod", "Ctrl", modBuffer, sizeof(modBuffer), filename);
    GetPrivateProfileString("Window", "TopkeyVk", "Space", buffer, sizeof(buffer), filename);
    config->topkey.modifiers = ParseModifiers(modBuffer);
    config->topkey.vk = GetVkFromName(buffer);

    // Load transkey
    GetPrivateProfileString("Window", "TransKeyMod", "Ctrl+Shift", modBuffer, sizeof(modBuffer), filename);
    GetPrivateProfileString("Window", "TransKeyVk", "Space", buffer, sizeof(buffer), filename);
    config->transkey.modifiers = ParseModifiers(modBuffer);
    config->transkey.vk = GetVkFromName(buffer);

    // Load keyboard mappings
    // First, remove the default mapping we added
    ConfigRemoveAllMappings(config);

    // Try to read mappings from [Keyboard] section
    FILE* file = fopen(filename, "r");
    if (!file) {
        // If file doesn't exist, keep default mapping
        ConfigAddMapping(config, "CapsLock", "Escape");
        return FALSE;
    }

    char line[1024];
    char currentSection[MAX_PATH_LENGTH] = "";
    BOOL inKeyboardSection = FALSE;

    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[--len] = '\0';
        }

        // Skip empty lines and comments
        if (len == 0 || line[0] == ';' || line[0] == '#') continue;

        // Check for section header
        if (line[0] == '[' && line[len-1] == ']') {
            strncpy(currentSection, line + 1, len - 2);
            currentSection[len - 2] = '\0';
            inKeyboardSection = (stricmp(currentSection, "Keyboard") == 0);
            continue;
        }

        // If we're in the keyboard section, parse mappings
        if (inKeyboardSection) {
            char* equals = strchr(line, '=');
            if (equals) {
                *equals = '\0';
                char* fromKey = TrimWhitespace(line);
                char* toKey = TrimWhitespace(equals + 1);
                if (strlen(fromKey) > 0 && strlen(toKey) > 0) {
                    ConfigAddMapping(config, fromKey, toKey);
                }
                *equals = '=';
            }
        }
    }

    fclose(file);

    // If no mappings were loaded, add default
    if (config->mappingCount == 0) {
        ConfigAddMapping(config, "CapsLock", "Escape");
    }

    return TRUE;
}

// Save configuration to INI file
BOOL ConfigSave(const AppConfig* config, const char* filename) {
    if (!config || !filename) return FALSE;

    // Save general settings
    WritePrivateProfileInt("General", "AutoStart", config->autoStart, filename);
    WritePrivateProfileInt("General", "LogLevel", config->logLevel, filename);
    WritePrivateProfileInt("General", "ShowNotifications", config->showNotifications, filename);

    // Save window settings
    WritePrivateProfileInt("Window", "EnableMidClick", config->enableMidClick, filename);

    // Save topkey
    const char* topkeyName = GetKeyName(config->topkey.vk);
    WritePrivateProfileString("Window", "TopkeyVk", topkeyName, filename);

    // Build topkey modifier string
    char modBuffer[128] = "";
    if (config->topkey.modifiers & MOD_CTRL) {
        if (modBuffer[0]) strcat(modBuffer, "+");
        strcat(modBuffer, "Ctrl");
    }
    if (config->topkey.modifiers & MOD_ALT) {
        if (modBuffer[0]) strcat(modBuffer, "+");
        strcat(modBuffer, "Alt");
    }
    if (config->topkey.modifiers & MOD_SHIFT) {
        if (modBuffer[0]) strcat(modBuffer, "+");
        strcat(modBuffer, "Shift");
    }
    if (config->topkey.modifiers & MOD_WIN) {
        if (modBuffer[0]) strcat(modBuffer, "+");
        strcat(modBuffer, "Win");
    }
    WritePrivateProfileString("Window", "TopkeyMod", modBuffer, filename);

    // Save transkey
    const char* transkeyName = GetKeyName(config->transkey.vk);
    WritePrivateProfileString("Window", "TransKeyVk", transkeyName, filename);

    // Build transkey modifier string
    modBuffer[0] = '\0';
    if (config->transkey.modifiers & MOD_CTRL) {
        if (modBuffer[0]) strcat(modBuffer, "+");
        strcat(modBuffer, "Ctrl");
    }
    if (config->transkey.modifiers & MOD_ALT) {
        if (modBuffer[0]) strcat(modBuffer, "+");
        strcat(modBuffer, "Alt");
    }
    if (config->transkey.modifiers & MOD_SHIFT) {
        if (modBuffer[0]) strcat(modBuffer, "+");
        strcat(modBuffer, "Shift");
    }
    if (config->transkey.modifiers & MOD_WIN) {
        if (modBuffer[0]) strcat(modBuffer, "+");
        strcat(modBuffer, "Win");
    }
    WritePrivateProfileString("Window", "TransKeyMod", modBuffer, filename);

    // Save keyboard mappings
    // First, clear any existing mappings in the file
    // We'll rewrite the [Keyboard] section
    for (int i = 0; i < config->mappingCount; i++) {
        WritePrivateProfileString("Keyboard", config->mappings[i].fromKey, config->mappings[i].toKey, filename);
    }

    return TRUE;
}

// Add a key mapping
BOOL ConfigAddMapping(AppConfig* config, const char* fromKey, const char* toKey) {
    if (!config || !fromKey || !toKey) return FALSE;
    if (config->mappingCount >= MAX_MAPPING_COUNT) return FALSE;

    // Check if mapping already exists
    for (int i = 0; i < config->mappingCount; i++) {
        if (stricmp(config->mappings[i].fromKey, fromKey) == 0) {
            // Update existing mapping
            strncpy(config->mappings[i].toKey, toKey, MAX_KEY_NAME_LENGTH - 1);
            config->mappings[i].toKey[MAX_KEY_NAME_LENGTH - 1] = '\0';
            return TRUE;
        }
    }

    // Add new mapping
    strncpy(config->mappings[config->mappingCount].fromKey, fromKey, MAX_KEY_NAME_LENGTH - 1);
    config->mappings[config->mappingCount].fromKey[MAX_KEY_NAME_LENGTH - 1] = '\0';
    strncpy(config->mappings[config->mappingCount].toKey, toKey, MAX_KEY_NAME_LENGTH - 1);
    config->mappings[config->mappingCount].toKey[MAX_KEY_NAME_LENGTH - 1] = '\0';
    config->mappingCount++;

    return TRUE;
}

// Remove a key mapping
BOOL ConfigRemoveMapping(AppConfig* config, const char* fromKey) {
    if (!config || !fromKey) return FALSE;

    for (int i = 0; i < config->mappingCount; i++) {
        if (stricmp(config->mappings[i].fromKey, fromKey) == 0) {
            // Shift remaining mappings
            for (int j = i; j < config->mappingCount - 1; j++) {
                config->mappings[j] = config->mappings[j + 1];
            }
            config->mappingCount--;
            return TRUE;
        }
    }

    return FALSE;
}

// Remove all key mappings
void ConfigRemoveAllMappings(AppConfig* config) {
    if (!config) return;
    config->mappingCount = 0;
    memset(config->mappings, 0, sizeof(config->mappings));
}