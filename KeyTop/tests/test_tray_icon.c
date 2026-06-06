#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../src/tray_icon.h"

// Test counter
static int tests_passed = 0;
static int tests_failed = 0;

// Helper macro for test results
#define TEST_ASSERT(condition, test_name) \
    do { \
        if (condition) { \
            printf("[PASS] %s\n", test_name); \
            tests_passed++; \
        } else { \
            printf("[FAIL] %s\n", test_name); \
            tests_failed++; \
        } \
    } while(0)

// Test functions
void test_tray_icon_null_instance(void) {
    printf("\nTest: NULL hInstance handling\n");

    // Attempt to create tray icon with NULL instance
    HWND hwnd = GetDesktopWindow(); // Use desktop window as a test HWND
    BOOL result = TrayIconCreate(NULL, hwnd);

    TEST_ASSERT(result == FALSE, "TrayIconCreate returns FALSE for NULL hInstance");

    // Test destroying with NULL hwnd
    TrayIconDestroy(NULL);
    TEST_ASSERT(TRUE, "TrayIconDestroy handles NULL hwnd gracefully");
}

void test_tray_icon_destroy_null_hwnd(void) {
    printf("\nTest: Destroy with NULL hwnd\n");

    // Call destroy with NULL hwnd - should not crash
    TrayIconDestroy(NULL);
    TEST_ASSERT(TRUE, "TrayIconDestroy(NULL) does not crash");
}

void test_tray_icon_set_null_callback(void) {
    printf("\nTest: Set NULL callback\n");

    // Call with NULL callback - should not crash
    TrayIconSetMenuCallback(NULL);
    TEST_ASSERT(TRUE, "TrayIconSetMenuCallback(NULL) does not crash");
}

void test_tray_icon_load_null_instance(void) {
    printf("\nTest: Load icon with NULL instance\n");

    // Attempt to load icon with NULL instance
    HICON hIcon = TrayIconLoadIcon(NULL, 1);

    TEST_ASSERT(hIcon == NULL, "TrayIconLoadIcon returns NULL for NULL hInstance");
}

int main(void) {
    printf("=== KeyTop Tray Icon Module Tests ===\n");

    test_tray_icon_null_instance();
    test_tray_icon_destroy_null_hwnd();
    test_tray_icon_set_null_callback();
    test_tray_icon_load_null_instance();

    printf("\n=== Test Results ===\n");
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);

    if (tests_failed > 0) {
        printf("\nSome tests FAILED!\n");
        return 1;
    }

    printf("\nAll tests PASSED!\n");
    return 0;
}