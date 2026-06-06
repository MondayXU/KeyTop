@echo off
setlocal

echo Building KeyTop release...

if not exist obj mkdir obj

cd obj

gcc -Wall -Wextra -O2 -Os -I../../include -c ../../src/main.c -o main.o
gcc -Wall -Wextra -O2 -Os -I../../include -c ../../src/utils.c -o utils.o
gcc -Wall -Wextra -O2 -Os -I../../include -c ../../src/config.c -o config.o
gcc -Wall -Wextra -O2 -Os -I../../include -c ../../src/window_manager.c -o window_manager.o
gcc -Wall -Wextra -O2 -Os -I../../include -c ../../src/keyboard_hook.c -o keyboard_hook.o
gcc -Wall -Wextra -O2 -Os -I../../include -c ../../src/mouse_hook.c -o mouse_hook.o
gcc -Wall -Wextra -O2 -Os -I../../include -c ../../src/hotkey.c -o hotkey.o
gcc -Wall -Wextra -O2 -Os -I../../include -c ../../src/tray_icon.c -o tray_icon.o
gcc -Wall -Wextra -O2 -Os -I../../include -c ../../src/settings_gui.c -o settings_gui.o
gcc -Wall -Wextra -O2 -Os -I../../include -c ../../src/error_handler.c -o error_handler.o

gcc -Wall -Wextra -O2 -Os main.o utils.o config.o window_manager.o keyboard_hook.o mouse_hook.o hotkey.o tray_icon.o settings_gui.o error_handler.o -mwindows -lcomctl32 -lshell32 -luser32 -lgdi32 -o ../KeyTop.exe

if errorlevel 1 (
    echo Build failed!
    exit /b 1
)

cd ..

echo Build successful!
echo KeyTop.exe created in current directory
