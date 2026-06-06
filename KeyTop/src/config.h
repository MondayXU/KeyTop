#ifndef CONFIG_H
#define CONFIG_H

#include "keytop.h"

// Modifier constants
#define MOD_CTRL   0x0001
#define MOD_ALT    0x0002
#define MOD_SHIFT  0x0004
#define MOD_WIN    0x0008

typedef struct {
    UINT modifiers;
    UINT vk;
} Hotkey;

typedef struct {
    char fromKey[MAX_KEY_NAME_LENGTH];
    char toKey[MAX_KEY_NAME_LENGTH];
} KeyMapping;

typedef struct {
    int autoStart;
    int logLevel;
    int showNotifications;
    Hotkey topkey;
    Hotkey transkey;
    int enableMidClick;
    KeyMapping mappings[MAX_MAPPING_COUNT];
    int mappingCount;
} AppConfig;

BOOL ConfigLoad(AppConfig* config, const char* filename);
BOOL ConfigSave(const AppConfig* config, const char* filename);
void ConfigGetDefaults(AppConfig* config);
BOOL ConfigAddMapping(AppConfig* config, const char* fromKey, const char* toKey);
BOOL ConfigRemoveMapping(AppConfig* config, const char* fromKey);
void ConfigRemoveAllMappings(AppConfig* config);

#endif /* CONFIG_H */