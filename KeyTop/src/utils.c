#include "utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <shlobj.h>

static LogLevel currentLogLevel = LOG_INFO;
static char logFilePath[MAX_PATH] = {0};

void LogMessage(LogLevel level, const char* module, const char* format, ...)
{
    if (level > currentLogLevel) return;

    FILE* file = fopen(logFilePath, "a");
    if (!file) return;

    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    char timeStr[26];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);

    const char* levelStr[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    fprintf(file, "[%s] [%s] [%s] ", timeStr, levelStr[level], module);

    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fprintf(file, "\n");
    fclose(file);
}

void LogSetLevel(LogLevel level)
{
    currentLogLevel = level;
}

LogLevel LogGetLevel(void)
{
    return currentLogLevel;
}

void LogSetFile(const char* filename)
{
    strncpy(logFilePath, filename, MAX_PATH - 1);
}

char* GetConfigPath(void)
{
    static char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    char* lastSlash = strrchr(path, '\\');
    if (lastSlash) *(lastSlash + 1) = '\0';
    strcat(path, CONFIG_FILE);
    return path;
}

char* GetLogPath(void)
{
    static char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    char* lastSlash = strrchr(path, '\\');
    if (lastSlash) *(lastSlash + 1) = '\0';
    strcat(path, LOG_FILE);
    return path;
}

void EnsureConfigDirectory(void)
{
    char* configPath = GetConfigPath();
    char* lastSlash = strrchr(configPath, '\\');
    if (lastSlash) {
        *lastSlash = '\0';
        CreateDirectory(configPath, NULL);
    }
}