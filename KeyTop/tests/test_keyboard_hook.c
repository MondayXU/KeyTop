#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "keyboard_hook.h"
#include "config.h"

// Test counter
static int testsPassed = 0;
static int testsFailed = 0;

// Test helper macros
#define TEST_ASSERT(condition, message) do { \
    if (condition) { \
        printf("[PASS] %s\n", message); \
        testsPassed++; \
    } else { \
        printf("[FAIL] %s\n", message); \
        testsFailed++; \
    } \
} while(0)

// Test 1: NULL config returns FALSE
void test_keyboard_hook_init_null_config(void) {
    printf("\n--- Test: KeyboardHookInit with NULL config ---\n");
    BOOL result = KeyboardHookInit(NULL);
    TEST_ASSERT(result == FALSE, "KeyboardHookInit(NULL) should return FALSE");
}

// Test 2: Shutdown when uninitialized should not crash
void test_keyboard_hook_shutdown_uninitialized(void) {
    printf("\n--- Test: KeyboardHookShutdown when uninitialized ---\n");
    // This should not crash
    KeyboardHookShutdown();
    TEST_ASSERT(TRUE, "KeyboardHookShutdown() should not crash when uninitialized");
}

// Test 3: IsEnabled when uninitialized should return FALSE
void test_keyboard_hook_is_enabled_uninitialized(void) {
    printf("\n--- Test: KeyboardHookIsEnabled when uninitialized ---\n");
    BOOL result = KeyboardHookIsEnabled();
    TEST_ASSERT(result == FALSE, "KeyboardHookIsEnabled() should return FALSE when uninitialized");
}

// Test 4: ReloadConfig with NULL should not crash
void test_keyboard_hook_reload_null_config(void) {
    printf("\n--- Test: KeyboardHookReloadConfig with NULL config ---\n");
    // This should not crash
    KeyboardHookReloadConfig(NULL);
    TEST_ASSERT(TRUE, "KeyboardHookReloadConfig(NULL) should not crash");
}

// Test 5: SetCallback should not crash
void test_keyboard_hook_set_callback(void) {
    printf("\n--- Test: KeyboardHookSetCallback ---\n");
    // Test setting NULL callback
    KeyboardHookSetCallback(NULL);
    TEST_ASSERT(TRUE, "KeyboardHookSetCallback(NULL) should not crash");
    
    // Test setting a valid callback (we'll use a simple stub)
    KeyCallback testCallback = NULL; // Just testing the function call
    KeyboardHookSetCallback(testCallback);
    TEST_ASSERT(TRUE, "KeyboardHookSetCallback() should not crash");
}

int main(void) {
    printf("=== Keyboard Hook Module Tests ===\n");
    
    // Run all tests
    test_keyboard_hook_init_null_config();
    test_keyboard_hook_shutdown_uninitialized();
    test_keyboard_hook_is_enabled_uninitialized();
    test_keyboard_hook_reload_null_config();
    test_keyboard_hook_set_callback();
    
    // Print summary
    printf("\n=== Test Summary ===\n");
    printf("Tests passed: %d\n", testsPassed);
    printf("Tests failed: %d\n", testsFailed);
    printf("Total tests:  %d\n", testsPassed + testsFailed);
    
    if (testsFailed == 0) {
        printf("\n✓ All tests passed!\n");
        return 0;
    } else {
        printf("\n✗ Some tests failed!\n");
        return 1;
    }
}
