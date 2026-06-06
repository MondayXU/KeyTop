#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "keytop.h"
#include "config.h"

// Test counters
static int tests_run = 0;
static int tests_passed = 0;

#define TEST_ASSERT(condition, message) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        printf("✓ PASS: %s\n", message); \
    } else { \
        printf("✗ FAIL: %s\n", message); \
    } \
} while(0)

void test_config_defaults() {
    printf("\n=== Testing ConfigGetDefaults ===\n");

    AppConfig config;
    ConfigGetDefaults(&config);

    TEST_ASSERT(config.autoStart == 0, "autoStart should be 0");
    TEST_ASSERT(config.logLevel == LOG_INFO, "logLevel should be LOG_INFO (2)");
    TEST_ASSERT(config.showNotifications == 1, "showNotifications should be 1");
    TEST_ASSERT(config.enableMidClick == 1, "enableMidClick should be 1");
    TEST_ASSERT(config.topkey.modifiers == MOD_CTRL, "topkey modifiers should be Ctrl");
    TEST_ASSERT(config.topkey.vk == VK_SPACE, "topkey vk should be Space");
    TEST_ASSERT(config.transkey.modifiers == (MOD_CTRL | MOD_SHIFT), "transkey modifiers should be Ctrl+Shift");
    TEST_ASSERT(config.transkey.vk == VK_SPACE, "transkey vk should be Space");
    TEST_ASSERT(config.mappingCount == 1, "should have 1 default mapping");
    TEST_ASSERT(strcmp(config.mappings[0].fromKey, "CapsLock") == 0, "first mapping should be from CapsLock");
    TEST_ASSERT(strcmp(config.mappings[0].toKey, "Escape") == 0, "first mapping should be to Escape");
}

void test_config_add_mapping() {
    printf("\n=== Testing ConfigAddMapping ===\n");

    AppConfig config;
    ConfigGetDefaults(&config);

    // Add a new mapping
    BOOL result = ConfigAddMapping(&config, "Tab", "Enter");
    TEST_ASSERT(result == TRUE, "adding new mapping should succeed");
    TEST_ASSERT(config.mappingCount == 2, "should have 2 mappings after adding");
    TEST_ASSERT(strcmp(config.mappings[1].fromKey, "Tab") == 0, "second mapping should be from Tab");
    TEST_ASSERT(strcmp(config.mappings[1].toKey, "Enter") == 0, "second mapping should be to Enter");

    // Update existing mapping
    result = ConfigAddMapping(&config, "CapsLock", "Tab");
    TEST_ASSERT(result == TRUE, "updating existing mapping should succeed");
    TEST_ASSERT(config.mappingCount == 2, "should still have 2 mappings after update");
    TEST_ASSERT(strcmp(config.mappings[0].fromKey, "CapsLock") == 0, "first mapping should still be from CapsLock");
    TEST_ASSERT(strcmp(config.mappings[0].toKey, "Tab") == 0, "first mapping should now be to Tab");

    // Test invalid parameters
    TEST_ASSERT(ConfigAddMapping(NULL, "a", "b") == FALSE, "NULL config should fail");
    TEST_ASSERT(ConfigAddMapping(&config, NULL, "b") == FALSE, NULL fromKey should fail");
    TEST_ASSERT(ConfigAddMapping(&config, "a", NULL) == FALSE, "NULL toKey should fail");
}

void test_config_remove_mapping() {
    printf("\n=== Testing ConfigRemoveMapping ===\n");

    AppConfig config;
    ConfigGetDefaults(&config);
    ConfigAddMapping(&config, "Tab", "Enter");
    ConfigAddMapping(&config, "F1", "F2");

    TEST_ASSERT(config.mappingCount == 3, "should have 3 mappings");

    // Remove existing mapping
    BOOL result = ConfigRemoveMapping(&config, "Tab");
    TEST_ASSERT(result == TRUE, "removing existing mapping should succeed");
    TEST_ASSERT(config.mappingCount == 2, "should have 2 mappings after removal");
    TEST_ASSERT(strcmp(config.mappings[0].fromKey, "CapsLock") == 0, "first mapping should be CapsLock->Tab");
    TEST_ASSERT(strcmp(config.mappings[1].fromKey, "F1") == 0, "second mapping should be F1->F2");

    // Remove non-existing mapping
    result = ConfigRemoveMapping(&config, "NonExistent");
    TEST_ASSERT(result == FALSE, "removing non-existing mapping should fail");
    TEST_ASSERT(config.mappingCount == 2, "mapping count should not change");

    // Test invalid parameters
    TEST_ASSERT(ConfigRemoveMapping(NULL, "a") == FALSE, "NULL config should fail");
    TEST_ASSERT(ConfigRemoveMapping(&config, NULL) == FALSE, "NULL fromKey should fail");
}

void test_config_save_load() {
    printf("\n=== Testing ConfigSave/Load ===\n");

    // Create a test config
    AppConfig config;
    ConfigGetDefaults(&config);
    config.autoStart = 1;
    config.logLevel = LOG_DEBUG;
    config.showNotifications = 0;
    ConfigAddMapping(&config, "F1", "F2");
    ConfigAddMapping(&config, "F3", "F4");

    // Save to a test file
    const char* testFile = "test_keytop.ini";
    BOOL saveResult = ConfigSave(&config, testFile);
    TEST_ASSERT(saveResult == TRUE, "saving config should succeed");

    // Load into a new config
    AppConfig loadedConfig;
    BOOL loadResult = ConfigLoad(&loadedConfig, testFile);
    TEST_ASSERT(loadResult == TRUE, "loading config should succeed");

    // Verify loaded values
    TEST_ASSERT(loadedConfig.autoStart == 1, "autoStart should be 1");
    TEST_ASSERT(loadedConfig.logLevel == LOG_DEBUG, "logLevel should be LOG_DEBUG (3)");
    TEST_ASSERT(loadedConfig.showNotifications == 0, "showNotifications should be 0");
    TEST_ASSERT(loadedConfig.mappingCount == 3, "should have 3 mappings");
    TEST_ASSERT(strcmp(loadedConfig.mappings[0].fromKey, "CapsLock") == 0, "first mapping should be CapsLock->Escape");
    TEST_ASSERT(strcmp(loadedConfig.mappings[0].toKey, "Escape") == 0, "first mapping should be CapsLock->Escape");
    TEST_ASSERT(strcmp(loadedConfig.mappings[1].fromKey, "F1") == 0, "second mapping should be F1->F2");
    TEST_ASSERT(strcmp(loadedConfig.mappings[1].toKey, "F2") == 0, "second mapping should be F1->F2");
    TEST_ASSERT(strcmp(loadedConfig.mappings[2].fromKey, "F3") == 0, "third mapping should be F3->F4");
    TEST_ASSERT(strcmp(loadedConfig.mappings[2].toKey, "F4") == 0, "third mapping should be F3->F4");

    // Clean up
    remove(testFile);
}

int main() {
    printf("Running KeyTop Configuration Module Tests\n");
    printf("=========================================\n");

    test_config_defaults();
    test_config_add_mapping();
    test_config_remove_mapping();
    test_config_save_load();

    printf("\n=========================================\n");
    printf("Tests run: %d/%d\n", tests_passed, tests_run);

    if (tests_passed == tests_run) {
        printf("✓ All tests passed!\n");
        return 0;
    } else {
        printf("✗ Some tests failed!\n");
        return 1;
    }
}