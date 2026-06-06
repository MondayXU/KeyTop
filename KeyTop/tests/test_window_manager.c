#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../src/window_manager.h"

#define MAX_TEST_NAME_LENGTH 64

typedef struct {
    char name[MAX_TEST_NAME_LENGTH];
    int passed;
    int failed;
} TestResult;

void init_test_result(TestResult* result, const char* name) {
    strncpy(result->name, name, MAX_TEST_NAME_LENGTH - 1);
    result->name[MAX_TEST_NAME_LENGTH - 1] = '\0';
    result->passed = 0;
    result->failed = 0;
}

void print_test_result(TestResult* result) {
    printf("Test: %s\n", result->name);
    printf("  Passed: %d\n", result->passed);
    printf("  Failed: %d\n", result->failed);
    
    if (result->failed == 0) {
        printf("  Result: PASS\n\n");
    } else {
        printf("  Result: FAIL\n\n");
    }
}

void test_window_get_foreground(TestResult* result) {
    init_test_result(result, "test_window_get_foreground");
    
    HWND foreground = WindowGetForegroundWindow();
    if (foreground != NULL) {
        printf("  Foreground window handle: %p\n", foreground);
        result->passed++;
    } else {
        printf("  Failed: GetForegroundWindow returned NULL\n");
        result->failed++;
    }
}

void test_window_is_topmost_invalid(TestResult* result) {
    init_test_result(result, "test_window_is_topmost_invalid");
    
    BOOL isTopmost = WindowIsTopmost(NULL);
    if (!isTopmost) {
        printf("  NULL window correctly returns FALSE for IsTopmost\n");
        result->passed++;
    } else {
        printf("  Failed: NULL window should return FALSE for IsTopmost\n");
        result->failed++;
    }
}

void test_window_toggle_topmost_invalid(TestResult* result) {
    init_test_result(result, "test_window_toggle_topmost_invalid");
    
    BOOL success = WindowToggleTopmost(NULL);
    if (!success) {
        printf("  NULL window correctly returns FALSE for ToggleTopmost\n");
        result->passed++;
    } else {
        printf("  Failed: NULL window should return FALSE for ToggleTopmost\n");
        result->failed++;
    }
}

void test_window_is_transparent_invalid(TestResult* result) {
    init_test_result(result, "test_window_is_transparent_invalid");
    
    BOOL isTransparent = WindowIsTransparent(NULL);
    if (!isTransparent) {
        printf("  NULL window correctly returns FALSE for IsTransparent\n");
        result->passed++;
    } else {
        printf("  Failed: NULL window should return FALSE for IsTransparent\n");
        result->failed++;
    }
}

void test_window_get_info_invalid(TestResult* result) {
    init_test_result(result, "test_window_get_info_invalid");
    
    WindowInfo info;
    BOOL success = WindowGetInfo(NULL, &info);
    if (!success) {
        printf("  NULL window correctly returns FALSE for GetInfo\n");
        result->passed++;
    } else {
        printf("  Failed: NULL window should return FALSE for GetInfo\n");
        result->failed++;
    }
}

int main(void) {
    printf("=== Window Manager Tests ===\n\n");
    
    int total_tests = 5;
    int total_passed = 0;
    int total_failed = 0;
    
    TestResult test1, test2, test3, test4, test5;
    
    test_window_get_foreground(&test1);
    test_window_is_topmost_invalid(&test2);
    test_window_toggle_topmost_invalid(&test3);
    test_window_is_transparent_invalid(&test4);
    test_window_get_info_invalid(&test5);
    
    print_test_result(&test1);
    print_test_result(&test2);
    print_test_result(&test3);
    print_test_result(&test4);
    print_test_result(&test5);
    
    total_passed = test1.passed + test2.passed + test3.passed + test4.passed + test5.passed;
    total_failed = test1.failed + test2.failed + test3.failed + test4.failed + test5.failed;
    
    printf("=== Summary ===\n");
    printf("Total Tests: %d\n", total_tests);
    printf("Total Passed: %d\n", total_passed);
    printf("Total Failed: %d\n", total_failed);
    
    if (total_failed == 0) {
        printf("\nAll tests PASSED!\n");
        return 0;
    } else {
        printf("\nSome tests FAILED!\n");
        return 1;
    }
}
