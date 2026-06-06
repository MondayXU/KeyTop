# Contributing to KeyTop

Thank you for your interest in contributing to KeyTop! This document provides guidelines and instructions for contributing to this project.

## Table of Contents

- [How to Report Bugs](#how-to-report-bugs)
- [How to Suggest Features](#how-to-suggest-features)
- [How to Submit Pull Requests](#how-to-submit-pull-requests)
- [Development Setup](#development-setup)
- [Coding Style Guidelines](#coding-style-guidelines)
- [Testing Requirements](#testing-requirements)
- [Community Guidelines](#community-guidelines)

## How to Report Bugs

We use GitHub Issues to track bugs. Before creating a bug report, please:

1. **Search existing issues** to see if the bug has already been reported
2. **Check the documentation** to ensure it's not a configuration issue
3. **Try the latest version** to see if the bug has been fixed

### Creating a Bug Report

When creating a bug report, please include:

- **Clear title** describing the issue
- **Description** of what happened vs. what you expected
- **Steps to reproduce** the issue
- **Environment details**:
  - Windows version (e.g., Windows 11 23H2)
  - KeyTop version
  - Any relevant system configuration
- **Screenshots** if applicable
- **Log files** if available (enable debug logging with `LogLevel=3`)

### Example Bug Report

```markdown
**Title:** KeyTop crashes when pressing Ctrl+Space

**Description:**
When I press Ctrl+Space to toggle topmost, KeyTop crashes and disappears from the system tray.

**Steps to reproduce:**
1. Launch KeyTop
2. Open any application
3. Press Ctrl+Space
4. KeyTop crashes

**Expected behavior:**
Window should toggle between topmost and normal state.

**Environment:**
- Windows 11 23H2 (Build 22631)
- KeyTop v1.0.0
- No other applications using Ctrl+Space

**Logs:**
[Attach KeyTop.log with debug logging enabled]
```

## How to Suggest Features

We welcome feature suggestions! Before suggesting a feature:

1. **Check existing features** in the documentation
2. **Search existing issues** to see if it's been suggested
3. **Consider the scope** - KeyTop is designed to be lightweight and focused

### Creating a Feature Request

Feature requests should include:

- **Clear title** describing the feature
- **Description** of the feature and its benefits
- **Use cases** explaining how it would be used
- **Alternatives considered** if applicable
- **Mockups or examples** if applicable

## How to Submit Pull Requests

### Getting Started

1. **Fork the repository** on GitHub
2. **Clone your fork** locally:
   ```bash
   git clone https://github.com/yourusername/keytop.git
   cd keytop
   ```
3. **Create a branch** for your changes:
   ```bash
   git checkout -b feature/your-feature-name
   ```
   or for bug fixes:
   ```bash
   git checkout -b fix/bug-description
   ```

### Making Changes

1. **Make your changes** following the coding style guidelines
2. **Test your changes** thoroughly
3. **Update documentation** if needed
4. **Commit your changes** with a clear commit message:
   ```bash
   git add .
   git commit -m "feat: add new feature description"
   ```

### Commit Message Format

We follow [Conventional Commits](https://www.conventionalcommits.org/):

- `feat: add new feature`
- `fix: resolve bug`
- `docs: update documentation`
- `style: formatting changes`
- `refactor: code refactoring`
- `test: add or update tests`
- `chore: maintenance tasks`

### Pull Request Process

1. **Push your branch** to your fork:
   ```bash
   git push origin feature/your-feature-name
   ```
2. **Create a Pull Request** on GitHub
3. **Fill out the PR template** with:
   - Description of changes
   - Related issues (if any)
   - Testing performed
   - Screenshots (if applicable)
4. **Wait for review** - maintainers will review and provide feedback

### PR Checklist

- [ ] Code follows the project's coding style
- [ ] All tests pass
- [ ] New tests added for new features
- [ ] Documentation updated if needed
- [ ] No breaking changes (or clearly documented)
- [ ] Commit messages follow conventional commits
- [ ] Branch is up to date with main

## Development Setup

### Prerequisites

- **Windows 10 or later** (Windows 11 recommended)
- **GCC compiler** (MinGW-w64 or Cygwin)
- **Git** for version control
- **Visual Studio Code** or similar IDE (optional)

### Setting Up the Development Environment

1. **Install MinGW-w64**:
   - Download from [MinGW-w64](https://www.mingw-w64.org/)
   - Add to system PATH
   - Verify installation: `gcc --version`

2. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/keytop.git
   cd keytop
   ```

3. **Explore the project structure**:
   ```
   KeyTop/
   ├── src/           # Source code
   ├── include/       # Header files
   ├── tests/         # Unit tests
   ├── docs/          # Documentation
   └── build/         # Build scripts
   ```

### Building from Command Line

```bash
cd KeyTop/build
make
```

Or using the batch script:
```bash
cd KeyTop/build
build.bat
```

### Running Tests

```bash
cd KeyTop/build
make test
```

## Coding Style Guidelines

### General Principles

- **Keep it simple**: KeyTop is designed to be lightweight and focused
- **Follow existing conventions**: Match the style of surrounding code
- **Use meaningful names**: Variables and functions should be self-documenting
- **Comment complex logic**: Explain "why", not "what"

### C Coding Style

#### Indentation and Formatting

- Use **4 spaces** for indentation (no tabs)
- Maximum line length: **100 characters**
- Opening braces on the **same line** as the statement
- One space after keywords (`if`, `for`, `while`, etc.)

```c
// Good
if (condition) {
    do_something();
}

// Bad
if(condition)
{
do_something();
}
```

#### Naming Conventions

- **Variables**: `lowercase_with_underscores`
  ```c
  int window_count;
  char *config_path;
  ```

- **Functions**: `lowercase_with_underscores`
  ```c
  void toggle_topmost(void);
  int load_config(const char *path);
  ```

- **Constants**: `UPPERCASE_WITH_UNDERSCORES`
  ```c
  #define MAX_HOTKEYS 64
  #define VERSION "1.0.0"
  ```

- **Types**: `lowercase_with_underscores` for built-in, `CamelCase` for custom
  ```c
  typedef struct {
      char name[64];
      UINT modifiers;
      UINT vk_code;
  } Hotkey;
  ```

#### Header Files

- Use **include guards**:
  ```c
  #ifndef WINDOW_MANAGER_H
  #define WINDOW_MANAGER_H

  // Header content

  #endif /* WINDOW_MANAGER_H */
  ```

- Include only what you need
- Forward declare when possible to reduce dependencies

#### Error Handling

- Always check return values
- Use the project's error handling utilities
- Log errors with appropriate severity levels

```c
HWND hwnd = FindWindow(NULL, "Target");
if (!hwnd) {
    log_error("Failed to find target window");
    return false;
}
```

#### Memory Management

- Always free allocated memory
- Use the project's memory wrappers when available
- Check for NULL after allocation

```c
char *buffer = malloc(size);
if (!buffer) {
    log_error("Memory allocation failed");
    return false;
}

// Use buffer...

free(buffer);
```

### Documentation

- Document all public functions in header files
- Use Doxygen-style comments for complex functions
- Keep comments up to date with code changes

```c
/**
 * Toggle the topmost state of the specified window
 * @param hwnd Handle to the window
 * @return true if successful, false otherwise
 */
bool toggle_topmost(HWND hwnd);
```

## Testing Requirements

### Writing Tests

- All new features must include unit tests
- Tests should be in the `tests/` directory
- Test both success and failure cases
- Test edge cases and boundary conditions

### Running Tests

```bash
cd KeyTop/build
make test
```

### Test Structure

```c
#include <assert.h>
#include "test_utils.h"

void test_toggle_topmost() {
    // Arrange
    HWND hwnd = create_test_window();

    // Act
    bool result = toggle_topmost(hwnd);

    // Assert
    assert(result == true);

    // Cleanup
    destroy_test_window(hwnd);
}
```

### Test Naming Convention

- Use descriptive names: `test_function_scenario_result`
- Examples:
  - `test_load_config_valid_file_returns_true`
  - `test_hotkey_parse_invalid_format_returns_false`

### Code Coverage

- Aim for **80% or higher** code coverage
- Focus on critical paths and error handling
- Run tests before submitting PR

## Community Guidelines

### Communication

- Be respectful and constructive
- Use inclusive language
- Assume good intentions

### Recognition

- Contributors will be acknowledged in the README
- Significant contributions will be featured in release notes

### Questions?

If you have questions about contributing:
- Check the existing documentation
- Open an issue with the `question` label
- Reach out to maintainers

---

Thank you for contributing to KeyTop! Your help makes this project better for everyone.
