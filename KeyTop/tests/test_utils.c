#include <stdio.h>
#include <string.h>
#include "../src/utils.h"

int test_log_message(void)
{
    LogSetLevel(LOG_DEBUG);
    LogSetFile("test.log");
    LogMessage(LOG_INFO, "Test", "Test message %d", 42);

    FILE* file = fopen("test.log", "r");
    if (!file) return 0;

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file)) {
        if (strstr(buffer, "Test message 42")) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int test_get_config_path(void)
{
    char* path = GetConfigPath();
    if (!path) return 0;
    return (strlen(path) > 0) ? 1 : 0;
}

int main(void)
{
    int passed = 0;
    int total = 2;

    printf("Testing utils module...\n");

    if (test_log_message()) {
        printf("✓ test_log_message passed\n");
        passed++;
    } else {
        printf("✗ test_log_message failed\n");
    }

    if (test_get_config_path()) {
        printf("✓ test_get_config_path passed\n");
        passed++;
    } else {
        printf("✗ test_get_config_path failed\n");
    }

    printf("\n%d/%d tests passed\n", passed, total);
    return (passed == total) ? 0 : 1;
}