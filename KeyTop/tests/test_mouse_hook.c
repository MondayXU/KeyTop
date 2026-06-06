#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "mouse_hook.h"
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
void test_mouse_hook_init_null_config(void) {
    printf("\n--- Test: MouseHookInit with NULL config ---\n");
    BOOL result = MouseHookInit(NULL);
    TEST_ASSERT(result == FALSE, "MouseHookInit(NULL) should return FALSE");
}

// Test 2: Shutdown when uninitialized should not crash
void test_mouse_hook_shutdown_uninitialized(void) {
    printf("\n--- Test: MouseHookShutdown when uninitialized ---\n");
    // This should not crash
    MouseHookShutdown();
    TEST_ASSERT(TRUE, "MouseHookShutdown() should not crash when uninitialized");
}

// Test 3: IsEnabled when uninitialized should return FALSE
void test_mouse_hook_is_enabled_uninitialized(void) {
    printf("\n--- Test: MouseHookIsEnabled when uninitialized ---\n");
    BOOL result = MouseHookIsEnabled();
    TEST_ASSERT(result == FALSE, "MouseHookIsEnabled() should return FALSE when uninitialized");
}

// Test 4: ReloadConfig with NULL should not crash
void test_mouse_hook_reload_null_config(void) {
    printf("\n--- Test: MouseHookReloadConfig with NULL config ---\n");
    // This should not crash
    MouseHookReloadConfig(NULL);
    TEST_ASSERT(TRUE, "MouseHookReloadConfig(NULL) should not crash");
}

// Test 5: SetCallback should not crash
void test_mouse_hook_set_callback(void) {
    printf("\n--- Test: MouseHookSetCallback ---\n");
    // Test setting NULL callback
    MouseHookSetCallback(NULL);
    TEST_ASSERT(TRUE, "MouseHookSetCallback(NULL) should not crash");

    // Test setting a valid callback (we'll use a simple stub)
    MouseCallback testCallback = NULL; // Just testing the function call
    MouseHookSetCallback(testCallback);
    TEST_ASSERT(TRUE, "MouseHookSetCallback() should not crash");
}

int main(void) {
    printf("=== Mouse Hook Module Tests ===\n");

    // Run all tests
    test_mouse_hook_init_null_config();
    test_mouse_hook_shutdown_uninitialized();
    test_mouse_hook_is_enabled_uninitialized();
    test_mouse_hook_reload_null_config();
    test_mouse_hook_set_callback();

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