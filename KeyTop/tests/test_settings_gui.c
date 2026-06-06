#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "keytop.h"
#include "config.h"
#include "settings_gui.h"

// Test counter
static int tests_passed = 0;
static int tests_failed = 0;

// Test helper macro
#define TEST_ASSERT(expr, name) do { \
    if (expr) { \
        printf("PASS: %s\n", name); \
        tests_passed++; \
    } else { \
        printf("FAIL: %s\n", name); \
        tests_failed++; \
    } \
} while(0)

// Test: Verify NULL config returns FALSE
void test_settings_null_config(void) {
    printf("\nRunning test_settings_null_config...\n");

    // Try to create settings with NULL config - should return FALSE
    BOOL result = SettingsCreate(GetModuleHandle(NULL), NULL, NULL);
    TEST_ASSERT(!result, "SettingsCreate with NULL config returns FALSE");
}

// Test: Verify destroy when uninitialized doesn't crash
void test_settings_destroy_uninitialized(void) {
    printf("\nRunning test_settings_destroy_uninitialized...\n");

    // This should not crash even if called when dialog doesn't exist
    SettingsDestroy();
    TEST_ASSERT(TRUE, "SettingsDestroy on uninitialized dialog doesn't crash");
}

// Test: Verify IsVisible returns FALSE when uninitialized
void test_settings_is_visible_uninitialized(void) {
    printf("\nRunning test_settings_is_visible_uninitialized...\n");

    // Should return FALSE when dialog doesn't exist
    BOOL result = SettingsIsVisible();
    TEST_ASSERT(!result, "SettingsIsVisible returns FALSE when uninitialized");
}

// Test: Verify SettingsHide doesn't crash when uninitialized
void test_settings_hide_uninitialized(void) {
    printf("\nRunning test_settings_hide_uninitialized...\n");

    // This should not crash even if called when dialog doesn't exist
    SettingsHide();
    TEST_ASSERT(TRUE, "SettingsHide on uninitialized dialog doesn't crash");
}

// Test: Verify SettingsApply doesn't crash when uninitialized
void test_settings_apply_uninitialized(void) {
    printf("\nRunning test_settings_apply_uninitialized...\n");

    // This should not crash even if called when dialog doesn't exist
    SettingsApply();
    TEST_ASSERT(TRUE, "SettingsApply on uninitialized dialog doesn't crash");
}

int main(void) {
    printf("=== KeyTop Settings GUI Tests ===\n");

    // Run all tests
    test_settings_null_config();
    test_settings_destroy_uninitialized();
    test_settings_is_visible_uninitialized();
    test_settings_hide_uninitialized();
    test_settings_apply_uninitialized();

    // Print summary
    printf("\n=== Test Summary ===\n");
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    printf("Total tests: %d\n", tests_passed + tests_failed);

    return tests_failed == 0 ? 0 : 1;
}