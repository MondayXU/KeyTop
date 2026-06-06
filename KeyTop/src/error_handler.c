#include "error_handler.h"
#include "keytop.h"

void ErrorShowBalloon(const char* title, const char* message)
{
    LogMessage(LOG_WARNING, "UI", "Balloon: %s - %s", title, message);
}

void ErrorShowDialog(const char* title, const char* message)
{
    MessageBox(NULL, message, title, MB_OK | MB_ICONERROR);
    LogMessage(LOG_ERROR, "UI", "Dialog: %s - %s", title, message);
}