#ifndef UTILS_H
#define UTILS_H

#include "keytop.h"

typedef enum {
    LOG_ERROR = 0,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG
} LogLevel;

void LogMessage(LogLevel level, const char* module, const char* format, ...);
void LogSetLevel(LogLevel level);
LogLevel LogGetLevel(void);
void LogSetFile(const char* filename);

char* GetConfigPath(void);
char* GetLogPath(void);
void EnsureConfigDirectory(void);

#endif /* UTILS_H */