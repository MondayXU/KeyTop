#include <stdio.h>
#include <string.h>
#include "../src/hotkey.h"

// Mock callback function
static int callback_called = 0;
static int last_callback_id = -1;

void mock_callback(int id)
{
    callback_called++;
    last_callback_id = id;
}

// Test 1: Verify NULL instance is OK for init
int test_hotkey_init_null_instance()
{
    printf("Testing HotkeyInit with NULL instance...\n");
    BOOL result = HotkeyInit(NULL);
    
    // Should still succeed (just stores NULL)
    if (!result) {
        printf("  ✗ Failed: HotkeyInit(NULL) returned FALSE\n");
        return 0;
    }
    
    // Cleanup
    HotkeyShutdown();
    printf("  ✓ Passed\n");
    return 1;
}

// Test 2: Verify shutdown when uninitialized doesn't crash
int test_hotkey_shutdown_uninitialized()
{
    printf("Testing HotkeyShutdown when uninitialized...\n");
    
    // Should not crash
    HotkeyShutdown();
    
    printf("  ✓ Passed (no crash)\n");
    return 1;
}

// Test 3: Verify register returns FALSE when not initialized
int test_hotkey_register_null_instance()
{
    printf("Testing HotkeyRegister without initialization...\n");
    
    // Need to reset state first (in case previous test initialized it)
    HotkeyShutdown();
    
    // Try to register without initialization
    BOOL result = HotkeyRegister(1001, MOD_CTRL | MOD_ALT, 'T');
    
    if (result) {
        printf("  ✗ Failed: HotkeyRegister should return FALSE when not initialized\n");
        return 0;
    }
    
    printf("  ✓ Passed\n");
    return 1;
}

// Test 4: Verify set callback doesn't crash
int test_hotkey_set_callback()
{
    printf("Testing HotkeySetCallback...\n");
    
    // Should not crash with valid callback
    HotkeySetCallback(mock_callback);
    
    // Should not crash with NULL callback
    HotkeySetCallback(NULL);
    
    printf("  ✓ Passed (no crash)\n");
    return 1;
}

// Test 5: Verify process message returns FALSE for NULL message
int test_hotkey_process_null_message()
{
    printf("Testing HotkeyProcessMessage with NULL message...\n");
    
    BOOL result = HotkeyProcessMessage(NULL);
    
    if (result) {
        printf("  ✗ Failed: HotkeyProcessMessage(NULL) should return FALSE\n");
        return 0;
    }
    
    printf("  ✓ Passed\n");
    return 1;
}

int main()
{
    int passed = 0;
    int total = 5;
    
    printf("Running hotkey module tests...\n\n");
    
    if (test_hotkey_init_null_instance()) passed++;
    if (test_hotkey_shutdown_uninitialized()) passed++;
    if (test_hotkey_register_null_instance()) passed++;
    if (test_hotkey_set_callback()) passed++;
    if (test_hotkey_process_null_message()) passed++;
    
    printf("\n%d/%d tests passed\n", passed, total);
    
    if (passed == total) {
        printf("\n✓ All tests passed!\n");
        return 0;
    } else {
        printf("\n✗ Some tests failed\n");
        return 1;
    }
}
