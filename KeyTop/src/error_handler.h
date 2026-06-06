#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "utils.h"

#define CHECK_ERROR(condition, message) \
    if (!(condition)) { \
        LogMessage(LOG_ERROR, "Error", "%s (line %d): %s", __FILE__, __LINE__, message); \
        return FALSE; \
    }

#define CHECK_ERROR_PTR(condition, message) \
    if (!(condition)) { \
        LogMessage(LOG_ERROR, "Error", "%s (line %d): %s", __FILE__, __LINE__, message); \
        return NULL; \
    }

#define SAFE_FREE(ptr) \
    if (ptr) { \
        free(ptr); \
        ptr = NULL; \
    }

void ErrorShowBalloon(const char* title, const char* message);
void ErrorShowDialog(const char* title, const char* message);

#endif /* ERROR_HANDLER_H */