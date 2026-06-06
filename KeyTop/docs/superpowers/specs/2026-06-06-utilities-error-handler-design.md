# KeyTop Task 2: Utilities and Error Handling Module Design

## Overview

Task 2 implements the utilities and error handling module for KeyTop, a lightweight Windows keyboard remapping and window management tool. This module provides logging, path management, and error handling functionality.

## Requirements Analysis

The module needs to provide:
1. **Logging system** with configurable levels (Error, Warning, Info, Debug)
2. **Path management** for configuration and log files
3. **Error handling** with macros and user notification functions
4. **Test suite** to validate functionality

## Architecture

### Components

#### 1. Utils Module (`src/utils.h` and `src/utils.c`)
- **LogLevel enum**: Four logging levels (ERROR, WARNING, INFO, DEBUG)
- **LogMessage()**: Core logging function with variable arguments
- **LogSetLevel()/LogGetLevel():** Log level management
- **LogSetFile()**: Set log file path
- **GetConfigPath()**: Get path to configuration file
- **GetLogPath()**: Get path to log file
- **EnsureConfigDirectory()**: Create config directory if needed

#### 2. Error Handler Module (`src/error_handler.h` and `src/error_handler.h`)
- **CHECK_ERROR macro**: Check condition and return FALSE on failure
- **CHECK_ERROR_PTR macro**: Check condition and return NULL on failure
- **SAFE_FREE macro**: Safe memory deallocation
- **ErrorShowBalloon()**: Show system tray balloon notification
- **ErrorShowDialog()**: Show error dialog box

#### 3. Test Suite (`tests/test_utils.c`)
- **test_log_message()**: Test logging functionality
- **test_get_config_path()**: Test path retrieval
- **main()**: Run all tests and report results

## Data Flow

1. **Logging Flow**:
   - Application calls LogMessage() with level, module, and format string
   - LogMessage checks if level is enabled
   - Opens log file, writes timestamp and formatted message
   - Closes file

2. **Path Management Flow**:
   - GetConfigPath/GetLogPath use GetModuleFileName to get executable path
   - Append CONFIG_FILE/LOG_FILE constants
   - Return static buffer with full path

3. **Error Handling Flow**:
   - Macros check conditions and log errors automatically
   - SAFE_FREE prevents dangling pointers
   - ErrorShowBalloon/ErrorShowDialog provide user feedback

## Dependencies

- **Windows API**: For GetModuleFileName, CreateDirectory, MessageBox
- **Standard C Library**: For file I/O, time functions, variable arguments
- **keytop.h**: For constants and type definitions

## Error Handling

- File operations use error checking
- Macros provide consistent error reporting
- Logging system itself has fallback behavior (silent failure if log file unavailable)

## Testing Strategy

1. **Unit tests** for each public function
2. **File-based testing** for logging (write then read back)
3. **Path validation** for configuration functions
4. **Return value checking** for all functions

## Implementation Notes

- Use static buffers for path functions (thread-safety not required)
- Log file opened/closed on each write (simple but safe)
- Macros use __FILE__ and __LINE__ for debugging information
- All functions follow Windows API conventions