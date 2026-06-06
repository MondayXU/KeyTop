# KeyTop

A lightweight Windows system tray tool for keyboard remapping and window management using pure Win32 API.

## Features

- **Keyboard Remapping**: Remap any key to another key or key combination
- **Window Management**:
  - Toggle window "always on top" with hotkey or middle-click
  - Toggle window transparency with hotkey
- **System Tray**: Runs quietly in the system tray with easy access to features
- **Hotkey Support**: Customizable hotkeys for all actions
- **Settings GUI**: Easy-to-use graphical settings interface
- **Configuration**: INI-based configuration with auto-start option
- **Logging**: Built-in logging for debugging and monitoring

## Usage

### Installation
1. Download the latest release
2. Extract to any folder
3. Run `KeyTop.exe`

### First Run
1. KeyTop will start and appear as an icon in the system tray
2. Right-click the tray icon or double-click to access the menu
3. Select "Settings" to configure your preferences

### Default Hotkeys
- **Ctrl+Alt+T**: Toggle "always on top" for the active window
- **Ctrl+Alt+R**: Toggle transparency for the active window
- **Middle-click**: Toggle "always on top" for the window under cursor

### System Tray Menu
- **Settings**: Open the settings GUI
- **Keyboard Hook**: Enable/disable keyboard remapping
- **Mouse Hook**: Enable/disable middle-click functionality
- **About**: Show version information
- **Exit**: Close KeyTop

## Configuration

KeyTop uses an INI file (`KeyTop.ini`) for configuration. An example file is provided as `KeyTop.ini.example`.

### Configuration Options
```ini
[General]
autoStart=0          ; Start with Windows (1=enabled, 0=disabled)
logLevel=1           ; Log level (0=none, 1=info, 2=debug)
showNotifications=1  ; Show balloon notifications (1=enabled, 0=disabled)

[Hotkeys]
topkey_modifiers=3   ; Always on top hotkey modifiers (1=Ctrl, 2=Alt, 4=Shift, 8=Win)
topkey_vk=84         ; Always on top virtual key code (T)
transkey_modifiers=3 ; Transparency hotkey modifiers
transkey_vk=82       ; Transparency virtual key code (R)

[Mouse]
enableMidClick=1     ; Enable middle-click to toggle topmost (1=enabled, 0=disabled)
```

## Building from Source

### Requirements
- Windows SDK
- MinGW-w64 or similar C compiler
- Make

### Build Instructions
```bash
cd KeyTop/build
make clean
make
```

The compiled executable will be created as `KeyTop.exe` in the build directory.

### Build Options
- `make clean`: Clean build artifacts
- `make all`: Build the executable (default)
- `make debug`: Build with debug flags (if available)

## Technical Details

### Architecture
KeyTop is built with a modular architecture:
- **Core**: Main program and window procedure
- **Hooks**: Low-level keyboard and mouse hooks
- **Hotkey**: Global hotkey registration and processing
- **Tray Icon**: System tray integration and menu handling
- **Settings**: Configuration GUI and management
- **Window Manager**: Window manipulation functions
- **Configuration**: INI file parsing and management
- **Utilities**: Logging, error handling, and helper functions

### Dependencies
- Windows API (user32, kernel32, shell32, gdi32)
- Common Controls (comctl32)
- No external libraries required

### System Requirements
- Windows XP or later
- No additional runtime requirements

## Troubleshooting

### KeyTop won't start
- Check the log file (`KeyTop.log`) for error messages
- Ensure no other application is using the same hotkeys
- Run as administrator if needed for certain features

### Hotkeys not working
- Check if keyboard hook is enabled in the tray menu
- Verify hotkeys aren't conflicting with other applications
- Try different modifier combinations

### Window management features not working
- Some windows may not support transparency or topmost state
- Try running KeyTop as administrator
- Check the log file for specific error messages

## License

KeyTop is released under the MIT License. See LICENSE file for details.

## Contributing

Contributions are welcome! Please submit issues and pull requests through the project repository.

## Changelog

### Version 1.0.0
- Initial release
- Keyboard remapping functionality
- Window management (topmost, transparency)
- System tray integration
- Settings GUI
- Configuration file support