@echo off
REM Build script for KeyTop
REM Requires MinGW or similar GCC toolchain

set CC=gcc
set CFLAGS=-Wall -Wextra -O2 -Os
set LDFLAGS=-mwindows -lcomctl32 -lshell32
set TARGET=KeyTop.exe
set SRCDIR=..\src
set INCDIR=..\include
set OBJDIR=obj

if not exist %OBJDIR% mkdir %OBJDIR%

echo Building KeyTop...
%CC% %CFLAGS% -I%INCDIR% -c %SRCDIR%\main.c -o %OBJDIR%\main.o
%CC% %CFLAGS% %OBJDIR%\main.o %LDFLAGS% -o %TARGET%

if exist %TARGET% (
    echo Build successful: %TARGET%
) else (
    echo Build failed!
    exit /b 1
)
